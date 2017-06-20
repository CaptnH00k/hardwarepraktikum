library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity ultrasound_controller is port(

	--the 50Mhz clock
	clk_50: in std_logic;
	
	-- enable signal
	enable : in std_logic;
	
	-- trigger, connected to ultrasound sensor
	trigger: inout std_logic;

	-- finished signal - high if system is idle
	finished : out std_logic;
		
	-- distance in cm (maximum = 255)
	distance : out std_logic_vector(7 downto 0)
);
end ultrasound_controller;



architecture behavior of ultrasound_controller is
	type state_type is (init, impulse, sensor_answer, distance_calc, distance_alloc);
	
	-- the current state of the FSM
	signal state : state_type := init;
	
	-- signalizes whether the sensor is currently writing onto the bidir pin trigger.
	signal gotImpulse : std_logic := '0';
	
	-- used to count the time 
	signal impulseHighCounter : unsigned (9 downto 0) := (others => '0');
	signal timeoutCounter : unsigned (20 downto 0) := (others => '0');
	signal impulseCounter : unsigned (20 downto 0) := (others => '0');
	signal distanceSignal : unsigned (20 downto 0) := (others => '0');

begin
	process(clk_50, trigger)
		begin
			if(rising_edge(clk_50)) then
				if state = init then
					finished <= '1';
					if enable = '1' then
						finished <= '0';
						-- since enable was 1 we switch to the next state.
						state <= impulse;
					end if;
						
				elsif state = impulse then
					-- We have to set the trigger to LOW first. We set it here to LOW for 1/250_000 seconds for a little margin.
					if impulseHighCounter < 200 then
						trigger <= '0';
						impulseHighCounter <= impulseHighCounter + 1;
						
					-- since the clk runs at 50Mhz and we want to wait 10 microseconds which are 1/100000 seconds, 
					-- we have to wait 50_000_000 / 100_000 rising edges which are 500. We add a puffer of 1 microsecond, thus the interval of 600 rising edges.
					elsif ((impulseHighCounter < 800) and (impulseHighCounter >= 200)) then
						-- We set the trigger impulse to HIGH for 11ms here.
						trigger <= '1';
						impulseHighCounter <= impulseHighCounter + 1;
					
					-- same here as above, we add a little puffer of 1/250_000 seconds.
					elsif impulseHighCounter < 1000 and impulseHighCounter >= 800 then
						trigger <= '0';
						impulseHighCounter <= impulseHighCounter + 1;
						
					else
						-- now we waited for ~14us so we reset the counter (for possible next runs) and set the trigger to high impedence since now the pin is an input.
						impulseHighCounter <= (others => '0');
						trigger <= 'Z';
						state <= sensor_answer;
						
					end if;
				
				elsif state = sensor_answer then
				-- since we have a timeout of 30ms which are 0.03 seconds we have to wait exactly (50Mhz * 0.03s) rising edges until a timout occurs, which is 1_500_000
					if ((timeoutCounter < 1500000) and (gotImpulse = '0')) then
						timeoutCounter <= timeoutCounter + 1;
						
						if trigger = '1' then
							gotImpulse <= '1';
						end if;
						
					elsif ((timeoutCounter = 1500000) and (gotImpulse = '0')) then
						-- a timeout occured, make sure to reset the counter and switch back to the initial state.
						timeoutCounter <= (others => '0');
						state <= init;
						
					elsif gotImpulse = '1' then
						-- the impulse has started make sure to increment the impulseCounter
						if trigger = '0' then
							gotImpulse <= '0';
							timeoutCounter <= (others => '0');
							impulseCounter <= impulseCounter  / 50;
							state <= distance_calc;
							
						 else
							impulseCounter <= impulseCounter + 1;
						
						end if;
					end if;
					
				elsif state = distance_calc then
					-- since we have to divide and cut the upper bits we have to save the temporary result in distanceSignal, 
					-- thus the need for one more state, since otherwise the code would get run at the same time, resulting in the distance vector being 0.
					distanceSignal <= impulseCounter / 58;
					impulseCounter <= (others => '0');
					state <= distance_alloc;
					
				elsif state = distance_alloc then
					-- make sure to cut the upper bits, so only the 8 lowest bits are in the wanted distance vector and switch back to the initial state.
					distance <= std_logic_vector(distanceSignal(7 downto 0));
					distanceSignal <= (others => '0');
					state <= init;
					
				end if;
			end if;
	end process;
end behavior;
