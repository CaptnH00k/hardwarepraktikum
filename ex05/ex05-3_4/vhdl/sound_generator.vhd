-- sound_generator
-- reads a 50Mhz clock and switches the Ouput 800 times per second, resulting in a 400Hertz signal output.


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sound_generator is port(
	clk_50 : in std_logic;
	
	sound : out std_logic
);
end sound_generator;


architecture behavior of sound_generator is
	signal counter : std_logic_vector(16 downto 0) := "00000000000000000"; -- The internal 17bit counter used to count up every time the clock has a rising edge.
	
	-- The value 62500 which is exactly 50Mhz/(400Hz * 2) since we have to switch the bit status 2 times.
	constant CHANGE_SOUND_EVERY : std_logic_vector(16 downto 0) := "01111010000100100";
	
	-- The value 125000 which is exactly 50Mhz/400Hz, e.g. the number when to reset the bit completely.
	constant RESET_SOUND_EVERY : std_logic_vector(16 downto 0) := "11110100001001000";
	
begin
	process(clk_50, counter)
		begin
			if(rising_edge(clk_50)) then
				counter <= std_logic_vector(unsigned(counter) + 1);
				if counter >= CHANGE_SOUND_EVERY then
					sound <= '1';
					if counter >= RESET_SOUND_EVERY then
						counter <= "00000000000000000";
					end if;
				else
					sound <= '0';
				end if;
			end if;
		end process;
end behavior;