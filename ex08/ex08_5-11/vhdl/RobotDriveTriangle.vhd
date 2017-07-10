library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity RobotDriveTriangle is port(
clk_50 : in std_logic;
heading : in std_logic_vector(8 downto 0);

targetHeadingOut : out std_logic_vector(8 downto 0);
motorAspeed : out std_logic_vector(9 downto 0);
motorAdirection : out std_logic;
motorBspeed : out std_logic_vector(9 downto 0);
motorBdirection : out std_logic
);
end RobotDriveTriangle;

architecture behavior of RobotDriveTriangle is
	
	-- the states available to this FSM
	type state_type is (drive_forward, drive_curve);
	
	-- the current state, initialized with its initial state.
	signal state : state_type := drive_forward;
	
	-- a boolean determining if the target heading was reset this clock cycle. 
	-- We have to take this into account since the logic is different since 360 = 0.
	signal reset : std_logic := '0';
	
	-- the value in degree where the board is headed to until it countinues driving forward again
	signal targetHeading : unsigned(8 downto 0) := (others => '0');

	-- a constant value determining the speed of the motors used here.
	constant SLOW_SPEED : std_logic_vector(9 downto 0) := "0100000000";
	
	-- a timer which resets every 4 seconds. Used to drive forward for 4 seconds.
	signal drive_timer : unsigned(27 downto 0) := (others => '0');

begin
	process(clk_50)
		begin
			if(rising_edge(clk_50)) then
				if state = drive_forward then
					drive_timer <= drive_timer + 1;
					
					if drive_timer = 200000000 then
						drive_timer <= (others => '0');
						
						-- make sure to manage overflows accordingly.
						if unsigned(heading) + 90 >= 360 then
							targetHeading <= (unsigned(heading) + 90) - 360;
							reset <= '1';
							
						else
							targetHeading <= unsigned(heading) + 90;
							reset <= '0';
							
						end if;
	
						state <= drive_curve;
						
					else
						-- Drive forward code here
						motorAspeed <= SLOW_SPEED;
						motorBspeed <= SLOW_SPEED;
						
						motorAdirection <= '0';
						motorBdirection <= '1';
						
					end if;
					
				elsif state = drive_curve then
					-- now we have to check whether the targetHeading got reset, since different 
					-- logic has to be applied to these cases. Also add a little padding, to 
					-- fix overturning of the robot.
					if reset = '0' and unsigned(heading) < targetHeading - 8 then
						motorAspeed <= SLOW_SPEED;
						motorBspeed <= SLOW_SPEED;
						
						motorAdirection <= '0';
						motorBdirection <= '0';
						
					elsif reset = '1' then
						-- here aswell, add a little padding, to fix overturning of the robot.
						if unsigned(heading) > 200 or unsigned(heading) < targetHeading - 8 then
							motorAspeed <= SLOW_SPEED;
							motorBspeed <= SLOW_SPEED;
							
							motorAdirection <= '0';
							motorBdirection <= '0';
						else
							state <= drive_forward;
						end if;
						
					else
						state <= drive_forward;
						
					end if;
				end if;
			end if;
	end process;
	
	-- also set the heading out value for the LCD.
	targetHeadingOut <= std_logic_vector(targetHeading);

end behavior;