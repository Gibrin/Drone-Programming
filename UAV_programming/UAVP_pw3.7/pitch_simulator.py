#!/usr/bin/env python
# coding: utf-8

# In[4]:


#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Симулятор вращательного движения БЛА в канале тангажа
ИСПРАВЛЕННАЯ ВЕРСИЯ - без колебаний
"""

import numpy as np
import matplotlib.pyplot as plt


class VehicleRotationalDynamic:
    """Модель вращательной динамики БЛА"""

    def __init__(self, I_y, k_b, l, T_cmd, alpha_init, omega_init, theta_init):
        self.I_y = I_y
        self.k_b = k_b
        self.l = l
        self.T_cmd = T_cmd
        self.alpha = alpha_init
        self.omega = omega_init
        self.theta = theta_init

    def rightParts(self, alpha_cmd):
        """Расчёт углового ускорения"""
        half_T_over_kb = (self.T_cmd / self.k_b) / 2.0
        delta = (self.I_y * alpha_cmd) / (2.0 * self.k_b * self.l)

        omega1_sq = half_T_over_kb + delta
        omega2_sq = half_T_over_kb - delta

        omega1_sq = max(0.0, omega1_sq)
        omega2_sq = max(0.0, omega2_sq)

        M_y = self.k_b * self.l * (omega1_sq - omega2_sq)
        self.alpha = M_y / self.I_y

    def integrate(self, dt):
        """Интегрирование методом Эйлера"""
        self.omega += self.alpha * dt
        self.theta += self.omega * dt

    def calculateState(self, alpha_cmd, dt):
        self.rightParts(alpha_cmd)
        self.integrate(dt)

    def getTheta(self): return self.theta
    def getOmega(self): return self.omega
    def getAlpha(self): return self.alpha


class CascadePIDController:
    """Каскадный ПИД-регулятор (УПРОЩЁННЫЙ - без колебаний)"""

    def __init__(self, Kp_theta, Ki_theta, Kd_theta, omega_limit,
                 Kp_omega, Ki_omega, Kd_omega, alpha_limit):
        # Внешний контур
        self.Kp_theta = Kp_theta
        self.Ki_theta = Ki_theta
        self.Kd_theta = Kd_theta
        self.omega_limit = omega_limit
        self.error_theta_prev = 0.0
        self.integral_theta = 0.0
        self.integral_theta_max = 10.0  # Ограничение интеграла

        # Внутренний контур
        self.Kp_omega = Kp_omega
        self.Ki_omega = Ki_omega
        self.Kd_omega = Kd_omega
        self.alpha_limit = alpha_limit
        self.error_omega_prev = 0.0
        self.integral_omega = 0.0
        self.integral_omega_max = 10.0  # Ограничение интеграла

        self.desired_theta = 0.0

    def setDesiredTheta(self, theta_des):
        self.desired_theta = theta_des

    def compute(self, theta, omega, dt):
        """Вычисление управляющего воздействия"""
        # === Внешний контур: угол → скорость ===
        error_theta = self.desired_theta - theta
        
        # Интеграл с ограничением (anti-windup)
        self.integral_theta += error_theta * dt
        self.integral_theta = np.clip(self.integral_theta, 
                                       -self.integral_theta_max, 
                                       self.integral_theta_max)
        
        # Производная с фильтрацией
        derivative_theta = (error_theta - self.error_theta_prev) / dt if dt > 0 else 0
        
        omega_des = (self.Kp_theta * error_theta +
                     self.Ki_theta * self.integral_theta +
                     self.Kd_theta * derivative_theta)
        
        # Ограничение выхода
        omega_des = np.clip(omega_des, -self.omega_limit, self.omega_limit)
        self.error_theta_prev = error_theta

        # === Внутренний контур: скорость → ускорение ===
        error_omega = omega_des - omega
        
        # Интеграл с ограничением
        self.integral_omega += error_omega * dt
        self.integral_omega = np.clip(self.integral_omega,
                                       -self.integral_omega_max,
                                       self.integral_omega_max)
        
        # Производная
        derivative_omega = (error_omega - self.error_omega_prev) / dt if dt > 0 else 0
        
        alpha_des = (self.Kp_omega * error_omega +
                     self.Ki_omega * self.integral_omega +
                     self.Kd_omega * derivative_omega)
        
        # Ограничение выхода
        alpha_des = np.clip(alpha_des, -self.alpha_limit, self.alpha_limit)
        self.error_omega_prev = error_omega

        return alpha_des


class Simulator:
    """Запуск моделирования и визуализация"""

    def __init__(self, Tend, dt, controller, model):
        self.Tend = Tend
        self.dt = dt
        self.controller = controller
        self.model = model
        self.time_list = []
        self.theta_list = []
        self.omega_list = []
        self.alpha_list = []

    def runSimulation(self):
        time = 0.0
        while time <= self.Tend:
            self.time_list.append(time)
            self.theta_list.append(self.model.getTheta())
            self.omega_list.append(self.model.getOmega())
            self.alpha_list.append(self.model.getAlpha())

            alpha_cmd = self.controller.compute(
                self.model.getTheta(),
                self.model.getOmega(),
                self.dt
            )
            self.model.calculateState(alpha_cmd, self.dt)
            time += self.dt

    def showPlots(self, target_angle_deg):
        """Построение графиков"""
        theta_deg = np.degrees(self.theta_list)
        omega_deg_s = np.degrees(self.omega_list)
        alpha_deg_s2 = np.degrees(self.alpha_list)

        fig, axs = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

        # График 1: Угол
        axs[0].plot(self.time_list, theta_deg, 'b-', linewidth=2, label='Текущий')
        axs[0].axhline(y=target_angle_deg, color='r', linestyle='--', label='Целевой')
        axs[0].set_ylabel('Угол, град')
        axs[0].set_title('Переходный процесс в канале тангажа')
        axs[0].legend(loc='lower right')
        axs[0].grid(True, alpha=0.3)

        # График 2: Скорость
        axs[1].plot(self.time_list, omega_deg_s, 'g-', linewidth=2, label='ω')
        axs[1].set_ylabel('Скорость, град/с')
        axs[1].legend(loc='upper right')
        axs[1].grid(True, alpha=0.3)

        # График 3: Ускорение
        axs[2].plot(self.time_list, alpha_deg_s2, 'orange', linewidth=2, label='ε')
        axs[2].set_ylabel('Ускорение, град/с²')
        axs[2].set_xlabel('Время, с')
        axs[2].legend(loc='upper right')
        axs[2].grid(True, alpha=0.3)

        plt.tight_layout()
        plt.savefig('pitch_simulation_result.png', dpi=300, bbox_inches='tight')
        plt.show()

        # Статистика
        final_error = abs(theta_deg[-1] - target_angle_deg)
        print(f"\n{'='*40}")
        print(f"Целевой угол: {target_angle_deg:.2f}°")
        print(f"Конечный угол: {theta_deg[-1]:.2f}°")
        print(f"Статическая ошибка: {final_error:.3f}°")
        print(f"{'='*40}")
        if final_error <= 3.0:
            print("✅ Ошибка в пределах допустимого (≤ 3°)")
        else:
            print("❌ Ошибка превышает допустимую!")
        
        return final_error


def main():
    """Настройка параметров"""
    
    # Параметры БЛА
    I_y = 0.2
    k_b = 1.0e-5
    l = 0.25
    T_cmd = 10.0

    # Начальные условия
    theta0 = np.radians(0.0)
    omega0 = 0.0
    alpha0 = 0.0

    # Целевое положение
    target_angle_deg = 30.0
    target_angle_rad = np.radians(target_angle_deg)

    # Параметры симуляции
    dt = 0.01
    Tend = 10.0

    # === НАСТРОЕННЫЕ КОЭФФИЦИЕНТЫ (без колебаний) ===
    # Внешний контур - БОЛЕЕ МЯГКИЕ
    Kp_theta = 2.0
    Ki_theta = 0.5
    Kd_theta = 0.3  # УМЕНЬШЕНА дифференциальная составляющая!
    omega_limit = np.radians(100.0)  # 100 град/с

    # Внутренний контур - БОЛЕЕ МЯГКИЕ
    Kp_omega = 2.5
    Ki_omega = 0.8
    Kd_omega = 0.1  # УМЕНЬШЕНА дифференциальная составляющая!
    alpha_limit = 10.0  # рад/с²

    # Создание объектов
    model = VehicleRotationalDynamic(I_y, k_b, l, T_cmd,
                                      alpha0, omega0, theta0)

    controller = CascadePIDController(
        Kp_theta, Ki_theta, Kd_theta, omega_limit,
        Kp_omega, Ki_omega, Kd_omega, alpha_limit
    )
    controller.setDesiredTheta(target_angle_rad)

    sim = Simulator(Tend, dt, controller, model)

    # Запуск
    print(f"🚁 Запуск симуляции: переход от 0° к {target_angle_deg}°")
    sim.runSimulation()
    error = sim.showPlots(target_angle_deg)


if __name__ == "__main__":
    main()


# In[ ]:




