#!/usr/bin/env python
# coding: utf-8

# In[1]:


#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Симулятор квадрокоптера (4 мотора) с полной динамикой 6 DOF
Стабильная версия для прохождения маршрута
"""

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from dataclasses import dataclass


@dataclass
class UAVParams:
    m: float = 1.0
    g: float = 9.81
    Ixx: float = 0.02
    Iyy: float = 0.02
    Izz: float = 0.04
    l: float = 0.25
    b: float = 3.13e-5
    d: float = 7.5e-7
    Ir: float = 1e-5
    bw: float = 0.01
    bv: float = 0.1
    max_motor_speed: float = 450.0
    max_thrust: float = 15.0


@dataclass
class ControlParams:
    kp_pos: float = 0.04
    ki_pos: float = 0.0
    kd_pos: float = 0.02
    pos_limit: float = 1.0

    kp_vel: float = 0.12
    ki_vel: float = 0.0
    kd_vel: float = 0.012
    vel_limit: float = 1.0

    kp_angle: float = 0.10
    ki_angle: float = 0.0
    kd_angle: float = 0.03
    angle_limit: float = np.radians(12)

    kp_rate: float = 0.020
    ki_rate: float = 0.0
    kd_rate: float = 0.003
    rate_limit: float = np.radians(15)

    max_thrust: float = 14.0


class PIDController:
    def __init__(self, kp, ki, kd, min_out=-np.inf, max_out=np.inf):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.min_out = min_out
        self.max_out = max_out
        self.prev_error = 0.0
        self.integral = 0.0

    def reset(self):
        self.prev_error = 0.0
        self.integral = 0.0

    def compute(self, error, dt):
        self.integral += error * dt
        derivative = (error - self.prev_error) / dt if dt > 0 else 0
        output = self.kp * error + self.ki * self.integral + self.kd * derivative
        output = np.clip(output, self.min_out, self.max_out)
        if output >= self.max_out or output <= self.min_out:
            self.integral -= error * dt
        self.prev_error = error
        return output


class QuadrotorModel:
    def __init__(self, params: UAVParams):
        self.p = params
        self.I = np.diag([params.Ixx, params.Iyy, params.Izz])
        self.I_inv = np.linalg.inv(self.I)
        self.state = np.zeros(12)
        self.motor_speeds = np.zeros(4)

    def get_rotation_matrix(self):
        phi, theta, psi = self.state[3:6]
        cphi, sphi = np.cos(phi), np.sin(phi)
        ctheta, stheta = np.cos(theta), np.sin(theta)
        cpsi, spsi = np.cos(psi), np.sin(psi)
        return np.array([
            [ctheta*cpsi, ctheta*spsi, -stheta],
            [sphi*stheta*cpsi - cphi*spsi, sphi*stheta*spsi + cphi*cpsi, sphi*ctheta],
            [cphi*stheta*cpsi + sphi*spsi, cphi*stheta*spsi - sphi*cpsi, cphi*ctheta]
        ])

    def compute_input_torques(self, motor_omega):
        omega_sq = motor_omega ** 2
        tau_x = self.p.b * self.p.l * (omega_sq[3] - omega_sq[1])
        tau_y = self.p.b * self.p.l * (omega_sq[2] - omega_sq[0])
        tau_z = self.p.d * (omega_sq[1] + omega_sq[3] - omega_sq[0] - omega_sq[2])
        return np.array([tau_x, tau_y, tau_z])

    def compute_gyroscopic_torque(self):
        omega = self.state[9:12]
        omega_r = (self.motor_speeds[0] - self.motor_speeds[1] +
                   self.motor_speeds[2] - self.motor_speeds[3])
        L_r = np.array([0, 0, self.p.Ir * omega_r])
        return np.cross(omega, L_r)

    def compute_drag_torques(self):
        omega = self.state[9:12]
        tau_friction = self.p.bw * omega * np.abs(omega)
        tau_drag = 0.01 * omega
        return tau_drag + tau_friction

    def compute_linear_acceleration(self, motor_omega):
        v = self.state[6:9]
        omega = self.state[9:12]

        total_thrust = self.p.b * np.sum(motor_omega ** 2)
        f_thrust_body = np.array([0, 0, total_thrust])

        R = self.get_rotation_matrix()
        f_thrust_earth = R @ f_thrust_body

        f_gravity_earth = np.array([0, 0, -self.p.m * self.p.g])

        f_coriolis = -2 * self.p.m * np.cross(omega, v)

        v_horiz = np.array([v[0], v[1], 0])
        f_drag = 0.1 * v_horiz
        f_friction = self.p.bv * v * np.abs(v)

        total_force = f_thrust_earth + f_gravity_earth + f_coriolis - f_drag - f_friction
        return total_force / self.p.m

    def compute_angular_acceleration(self, motor_omega):
        omega = self.state[9:12]
        tau_input = self.compute_input_torques(motor_omega)
        gyro_term = np.cross(omega, self.I @ omega)
        tau_gyro = self.compute_gyroscopic_torque()
        tau_drag = self.compute_drag_torques()
        omega_dot = self.I_inv @ (tau_input - gyro_term - tau_gyro - tau_drag)
        return np.clip(omega_dot, -100, 100)

    def compute_euler_rates(self):
        phi, theta, psi = self.state[3:6]
        p, q, r = self.state[9:12]
        theta = np.clip(theta, -np.pi/2 + 0.1, np.pi/2 - 0.1)
        T = np.array([
            [1, np.sin(phi)*np.tan(theta), np.cos(phi)*np.tan(theta)],
            [0, np.cos(phi),               -np.sin(phi)],
            [0, np.sin(phi)/np.cos(theta), np.cos(phi)/np.cos(theta)]
        ])
        return T @ np.array([p, q, r])

    def right_hand_side(self, state, motor_omega):
        old_state = self.state.copy()
        self.state = state
        acc_lin = self.compute_linear_acceleration(motor_omega)
        acc_ang = self.compute_angular_acceleration(motor_omega)
        euler_dot = self.compute_euler_rates()
        deriv = np.array([
            state[6], state[7], state[8],
            euler_dot[0], euler_dot[1], euler_dot[2],
            acc_lin[0], acc_lin[1], acc_lin[2],
            acc_ang[0], acc_ang[1], acc_ang[2]
        ])
        self.state = old_state
        return deriv

    def integrate_rk4(self, motor_omega, dt):
        motor_omega = np.clip(motor_omega, 0, self.p.max_motor_speed)
        k1 = self.right_hand_side(self.state, motor_omega)
        k2 = self.right_hand_side(self.state + 0.5*dt*k1, motor_omega)
        k3 = self.right_hand_side(self.state + 0.5*dt*k2, motor_omega)
        k4 = self.right_hand_side(self.state + dt*k3, motor_omega)
        self.state += dt * (k1 + 2*k2 + 2*k3 + k4) / 6.0

        self.state[3] = np.clip(self.state[3], -np.pi/2 + 0.1, np.pi/2 - 0.1)
        self.state[4] = np.clip(self.state[4], -np.pi/2 + 0.1, np.pi/2 - 0.1)
        self.state[5] = np.clip(self.state[5], -np.pi, np.pi)
        self.state[6:9] = np.clip(self.state[6:9], -10, 10)
        self.state[9:12] = np.clip(self.state[9:12], -np.radians(60), np.radians(60))

        self.motor_speeds = motor_omega.copy()


class CascadeController:
    def __init__(self, control_params: ControlParams, uav_params: UAVParams):
        self.cp = control_params
        self.up = uav_params

        self.pid_pos_x = PIDController(control_params.kp_pos, control_params.ki_pos,
                                       control_params.kd_pos,
                                       -control_params.pos_limit, control_params.pos_limit)
        self.pid_pos_y = PIDController(control_params.kp_pos, control_params.ki_pos,
                                       control_params.kd_pos,
                                       -control_params.pos_limit, control_params.pos_limit)
        self.pid_pos_z = PIDController(control_params.kp_pos, control_params.ki_pos,
                                       control_params.kd_pos,
                                       -control_params.pos_limit, control_params.pos_limit)

        self.pid_vel_x = PIDController(control_params.kp_vel, control_params.ki_vel,
                                       control_params.kd_vel,
                                       -control_params.vel_limit, control_params.vel_limit)
        self.pid_vel_y = PIDController(control_params.kp_vel, control_params.ki_vel,
                                       control_params.kd_vel,
                                       -control_params.vel_limit, control_params.vel_limit)
        self.pid_vel_z = PIDController(control_params.kp_vel, control_params.ki_vel,
                                       control_params.kd_vel,
                                       -control_params.vel_limit, control_params.vel_limit)

        self.pid_roll = PIDController(control_params.kp_angle, control_params.ki_angle,
                                      control_params.kd_angle,
                                      -control_params.angle_limit, control_params.angle_limit)
        self.pid_pitch = PIDController(control_params.kp_angle, control_params.ki_angle,
                                       control_params.kd_angle,
                                       -control_params.angle_limit, control_params.angle_limit)
        self.pid_yaw = PIDController(control_params.kp_angle * 0.5, control_params.ki_angle,
                                     control_params.kd_angle, -np.pi, np.pi)

        self.pid_rate_p = PIDController(control_params.kp_rate, control_params.ki_rate,
                                        control_params.kd_rate,
                                        -control_params.rate_limit, control_params.rate_limit)
        self.pid_rate_q = PIDController(control_params.kp_rate, control_params.ki_rate,
                                        control_params.kd_rate,
                                        -control_params.rate_limit, control_params.rate_limit)
        self.pid_rate_r = PIDController(control_params.kp_rate * 0.5, control_params.ki_rate,
                                        control_params.kd_rate,
                                        -control_params.rate_limit, control_params.rate_limit)

        self.target_pos = np.array([0.0, 0.0, 0.0])
        self.target_yaw = 0.0

    def set_target(self, pos, yaw=0.0):
        self.target_pos = np.array(pos)
        self.target_yaw = yaw
        self.pid_pos_x.reset()
        self.pid_pos_y.reset()
        self.pid_pos_z.reset()

    def compute(self, state, dt):
        pos = state[0:3]
        phi, theta, psi = state[3:6]
        vel = state[6:9]
        p, q, r = state[9:12]

        # Позиция -> желаемая скорость
        err_pos = self.target_pos - pos
        des_vx = self.pid_pos_x.compute(err_pos[0], dt)
        des_vy = self.pid_pos_y.compute(err_pos[1], dt)
        des_vz = self.pid_pos_z.compute(err_pos[2], dt)
        des_vx = np.clip(des_vx, -1.0, 1.0)
        des_vy = np.clip(des_vy, -1.0, 1.0)
        des_vz = np.clip(des_vz, -0.6, 0.6)

        # Скорость -> желаемое ускорение
        err_vel = np.array([des_vx, des_vy, des_vz]) - vel
        des_ax = self.pid_vel_x.compute(err_vel[0], dt)
        des_ay = self.pid_vel_y.compute(err_vel[1], dt)
        des_az = self.pid_vel_z.compute(err_vel[2], dt) + self.up.g
        des_ax = np.clip(des_ax, -1.5, 1.5)
        des_ay = np.clip(des_ay, -1.5, 1.5)
        des_az = np.clip(des_az, 5.0, 12.0)   # Ключевое: не даём падать

        # Ускорение -> желаемые углы
        cpsi, spsi = np.cos(psi), np.sin(psi)
        des_pitch = (des_ax * cpsi + des_ay * spsi) / self.up.g
        des_roll  = (-des_ax * spsi + des_ay * cpsi) / self.up.g
        des_roll = np.clip(des_roll, -self.cp.angle_limit, self.cp.angle_limit)
        des_pitch = np.clip(des_pitch, -self.cp.angle_limit, self.cp.angle_limit)

        # Угол -> угловая скорость
        err_roll = np.clip(des_roll - phi, -np.pi, np.pi)
        err_pitch = np.clip(des_pitch - theta, -np.pi, np.pi)
        err_yaw = np.clip(self.target_yaw - psi, -np.pi, np.pi)

        des_p = self.pid_roll.compute(err_roll, dt)
        des_q = self.pid_pitch.compute(err_pitch, dt)
        des_r = self.pid_yaw.compute(err_yaw, dt)
        max_rate = np.radians(40)
        des_p = np.clip(des_p, -max_rate, max_rate)
        des_q = np.clip(des_q, -max_rate, max_rate)
        des_r = np.clip(des_r, -max_rate, max_rate)

        # Угловая скорость -> моменты
        err_p = des_p - p
        err_q = des_q - q
        err_r = des_r - r
        tau_x = self.pid_rate_p.compute(err_p, dt)
        tau_y = self.pid_rate_q.compute(err_q, dt)
        tau_z = self.pid_rate_r.compute(err_r, dt)

        # Тяга
        total_thrust = self.up.m * des_az
        total_thrust = np.clip(total_thrust, 0, self.cp.max_thrust)

        # Mixer
        b, d, l = self.up.b, self.up.d, self.up.l
        base_omega = np.sqrt(total_thrust / (4 * b)) if total_thrust > 0 else 0
        delta_x = tau_x / (2 * b * l) if l > 0 and b > 0 else 0
        delta_y = tau_y / (2 * b * l) if l > 0 and b > 0 else 0
        delta_z = tau_z / (4 * d) if d > 0 else 0

        omega = np.array([
            base_omega - delta_y - delta_z,
            base_omega + delta_x + delta_z,
            base_omega + delta_y - delta_z,
            base_omega - delta_x + delta_z
        ])
        omega = np.clip(omega, 0, self.up.max_motor_speed)
        return omega


class Simulator:
    def __init__(self, model, controller, dt=0.01):
        self.model = model
        self.controller = controller
        self.dt = dt
        self.time = 0.0
        self.history = {'t': [], 'pos': [], 'orient': [], 'vel': []}

    def run(self, waypoints, total_time=60.0):
        current_wp = 0
        threshold = 1.0
        if waypoints:
            self.controller.set_target(waypoints[0])
        while self.time <= total_time:
            state = self.model.state
            self.history['t'].append(self.time)
            self.history['pos'].append(state[0:3].copy())
            self.history['orient'].append(state[3:6].copy())
            self.history['vel'].append(state[6:9].copy())

            if current_wp < len(waypoints):
                pos = state[0:3]
                if np.linalg.norm(pos - waypoints[current_wp]) < threshold:
                    if current_wp < len(waypoints)-1:
                        current_wp += 1
                        self.controller.set_target(waypoints[current_wp])
                        print(f"✓ Точка {current_wp}: {waypoints[current_wp]}")

            motor_cmd = self.controller.compute(state, self.dt)
            self.model.integrate_rk4(motor_cmd, self.dt)
            self.time += self.dt
        self.print_stats()

    def print_stats(self):
        t = np.array(self.history['t'])
        pos = np.array(self.history['pos'])
        if len(pos) > 1:
            dist = np.sum(np.linalg.norm(np.diff(pos, axis=0), axis=1))
            avg_speed = dist / t[-1]
        else:
            dist, avg_speed = 0, 0
        print("\n" + "="*50)
        print("СТАТИСТИКА")
        print("="*50)
        print(f"Время: {t[-1]:.2f} с")
        print(f"Расстояние: {dist:.2f} м")
        print(f"Средняя скорость: {avg_speed:.3f} м/с")
        print(f"Конечная позиция: {pos[-1]}")
        if avg_speed >= 0.5:
            print("✅ Средняя скорость ≥ 0.5 м/с")
        else:
            print("❌ Средняя скорость < 0.5 м/с")

    def plot(self):
        t = np.array(self.history['t'])
        pos = np.array(self.history['pos'])
        orient = np.array(self.history['orient'])
        vel = np.array(self.history['vel'])

        fig = plt.figure(figsize=(14, 10))
        ax1 = fig.add_subplot(2, 3, 1, projection='3d')
        ax1.plot(pos[:, 0], pos[:, 1], pos[:, 2], 'b-', linewidth=2)
        ax1.set_title('3D траектория')
        ax2 = fig.add_subplot(2, 3, 2)
        ax2.plot(t, pos[:, 0], 'r-', label='X')
        ax2.plot(t, pos[:, 1], 'g-', label='Y')
        ax2.plot(t, pos[:, 2], 'b-', label='Z')
        ax2.legend(); ax2.grid(True)
        ax3 = fig.add_subplot(2, 3, 3)
        ax3.plot(t, np.degrees(orient[:, 0]), 'r-', label='Roll')
        ax3.plot(t, np.degrees(orient[:, 1]), 'g-', label='Pitch')
        ax3.plot(t, np.degrees(orient[:, 2]), 'b-', label='Yaw')
        ax3.legend(); ax3.grid(True)
        ax4 = fig.add_subplot(2, 3, 4)
        ax4.plot(t, vel[:, 0], 'r-', label='Vx')
        ax4.plot(t, vel[:, 1], 'g-', label='Vy')
        ax4.plot(t, vel[:, 2], 'b-', label='Vz')
        ax4.legend(); ax4.grid(True)
        ax5 = fig.add_subplot(2, 3, 5)
        ax5.plot(pos[:, 0], pos[:, 1], 'b-')
        ax5.set_title('XY проекция')
        ax5.axis('equal'); ax5.grid(True)
        ax6 = fig.add_subplot(2, 3, 6)
        ax6.plot(t, pos[:, 2], 'b-')
        ax6.set_title('Высота')
        ax6.grid(True)
        plt.tight_layout()
        plt.savefig('simulation_final.png', dpi=300)
        plt.show()


def main():
    uav_params = UAVParams()
    control_params = ControlParams()
    model = QuadrotorModel(uav_params)
    controller = CascadeController(control_params, uav_params)
    sim = Simulator(model, controller, dt=0.01)

    waypoints = [
        [0.0, 0.0, 2.0],
        [5.0, 0.0, 2.0],
        [5.0, 5.0, 2.5],
        [0.0, 5.0, 2.0],
        [0.0, 0.0, 2.0],
        [0.0, 0.0, 0.5]
    ]

    print("🚁 Запуск симуляции квадрокоптера (финальная стабильная версия)")
    print(f"Масса: {uav_params.m} кг, Тяга/Вес: {4*uav_params.b*uav_params.max_motor_speed**2/(uav_params.m*uav_params.g):.2f}")
    sim.run(waypoints, total_time=60.0)
    sim.plot()


if __name__ == "__main__":
    main()


# In[ ]: