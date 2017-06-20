-- motor_controller
-- take note, since there are ~1500 signal changes per second it can be hard to see the accuary of this program, 
-- thus I included a close up, showing the first 10 milliseconds of the run with 1024 speed value, which is exactly 
-- the half of the max value, result in 15 signal changes per 10 milliseconds which is equal to 1500 signal changes in 1000 milliseconds = 1 second.
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity motor_controller is port(
	clk_50 : in std_logic;
	speed : in std_logic_vector(9 downto 0);
	direction : in std_logic;
	
	motor_signal1 : out std_logic;
	motor_signal2 : out std_logic
);
end motor_controller;


architecture behavior of motor_controller is	
	-- This counter resets at 64. This number originates as follows: 66666, which is approx (50Mhz/750), the 750 originates from 1500 / 2, because the signal should change
	-- 1500 times per second and we set the signal twice per process run, thus we have to halve it to achieve the desiered effect. Now we have to take the 2047 possible speed
	-- values into consideration, thus 66666 / 2048 ~= 32. Since we now assign one signal at a time, we have to take 32 * 2 = 64. So every 64 rising edges we change our signal.
	signal pwmCounter : unsigned(6 downto 0) := (others => '0');
	
	-- This is the counter used to set motors to off or on. If the value of speed is high then the motor on signal gets often called, otherwise the motor off signal gets often called.
	signal relationCounter : unsigned(9 downto 0) := (others => '0');
	
begin
	process(clk_50)
	begin
		if rising_edge(clk_50) then
			if pwmCounter = 64 then
				
				pwmCounter <= (others => '0');
				
				if relationCounter = 2047 then
					relationCounter <= (others => '0');
					
				end if;
				
				if unsigned(speed) = 0 or relationCounter > unsigned(speed) then
					motor_signal1 <= '0';
					motor_signal2 <= '0';	
				
					
				elsif unsigned(speed) = 2047 or relationCounter <= unsigned(speed) then
					if direction = '0' then
						motor_signal1 <= '0';
						motor_signal2 <= '1';
						
					elsif direction = '1' then
						motor_signal1 <= '1';
						motor_signal2 <= '0';
						
					end if;					
				end if;			
				relationCounter <= relationCounter + 1;
				
			else
				pwmCounter <= pwmCounter + 1;
				
		end if;
	end if;
	end process;
end behavior;