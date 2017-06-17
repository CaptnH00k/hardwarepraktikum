-- stop_watch
-- simple implementation of a 6 bit stop watch which counts up every second by utilizing a 50Mhz clock. 
-- Is being controlled by button presses and signalizes its current status with LEDs.


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity stop_watch is port(
	clk_50 : in std_logic;
	startStop : in std_logic;
	rst : in std_logic;

	countOut : out std_logic_vector(5 downto 0)
);
end stop_watch;


architecture behavior of stop_watch is
	signal secondResetCounter : std_logic_vector(25 downto 0) := "00000000000000000000000000"; -- The counter which resets every second, used to increment the intern counter every second.
	signal internalCounterSecond : std_logic_vector(5 downto 0) := "000000"; -- The internal 6bit counter which increments every second.
	signal isPaused : std_logic := '0';
	
	constant RESET_PER_SECOND : std_logic_vector(25 downto 0) := "10111110101111000010000000"; -- The number of logical '1's per second set by the 50Mhz clock, e.g. 50_000_000 times.
begin
	process(startStop)
		begin
		if(startStop = '1') then
			isPaused <= (not isPaused);
		end if;
	end process;
	process(rst, clk_50)
		begin
		if(rst = '1') then
			secondResetCounter <= "00000000000000000000000000";
			internalCounterSecond <= "000000";
			
		elsif((rising_edge(clk_50)) and (isPaused = '0')) then
			secondResetCounter <= std_logic_vector(unsigned(secondResetCounter) + 1);
			if secondResetCounter >= RESET_PER_SECOND then
				secondResetCounter <= "00000000000000000000000000";
				internalCounterSecond <= std_logic_vector(unsigned(internalCounterSecond) + 1);
			end if;
		end if;
	end process;
	countOut <= internalCounterSecond;
end behavior;