-- The way this robot handles obstacles is by turning at full speed when one of the sensors sends a distance of less than 20cm. 
-- Otherwise the robot will just drive forward with half speed.
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity robot_controller is port(
	
	-- the distance measured by the ultrasonic sensors in centimeters.
	ultrasonic_1 : in std_logic_vector(7 downto 0);
	ultrasonic_2 : in std_logic_vector(7 downto 0);
	ultrasonic_3 : in std_logic_vector(7 downto 0);
	
	-- the speed and direction for the respective motor.
	motor_A_direction : out std_logic;
	motor_A_speed : out std_logic_vector(9 downto 0);
	motor_B_direction : out std_logic;
	motor_B_speed : out std_logic_vector(9 downto 0)

);
end robot_controller;



architecture behavior of robot_controller is

begin
	process(ultrasonic_1, ultrasonic_2, ultrasonic_3)
		begin
			-- Turn at full speed if there's an obstacle in range of 20cm.
			if unsigned(ultrasonic_1) <= 20 or unsigned(ultrasonic_2) <= 20 or unsigned(ultrasonic_3) <= 20 then
				motor_A_direction <= '1';
				motor_B_direction <= '1';
				motor_A_speed <= "1111111111";
				motor_B_speed <= "1111111111";
			
			-- Drive forward at half speed if there's no obstacle nearby.
			else 
				motor_A_direction <= '1';
				motor_B_direction <= '0';
				motor_A_speed <= "1000000000";
				motor_B_speed <= "1000000000";
			end if;
		end process;
end behavior;
