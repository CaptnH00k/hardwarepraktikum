-- multiple_sound_generator
-- reads a 50Mhz clock and is able to write different sounds by the buttons pressed.


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity multiple_sound_generator is port(
	clk_50 : in std_logic;
	buttonPressed : in std_logic_vector(4 downto 0);
	
	sound : out std_logic
);
end multiple_sound_generator;


architecture behavior of multiple_sound_generator is
	signal counter : std_logic_vector(17 downto 0) := "000000000000000000"; -- The internal 17bit counter used to count up every time the clock has a rising edge.
	
	-- The value 62500 which is exactly 50Mhz/(400Hz * 2) since we have to switch the bit status 2 times.
	constant CHANGE_SOUND_EVERY_400HZ : std_logic_vector(17 downto 0) := "001111010000100100";
	
	-- The value 125000 which is exactly 50Mhz/400Hz, e.g. the number when to reset the bit completely.
	constant RESET_SOUND_EVERY_400HZ : std_logic_vector(17 downto 0) := "011110100001001000";
	
	-- The value 3571 which is approx 50Mhz/(7Khz * 2).
	constant CHANGE_SOUND_EVERY_7KHZ : std_logic_vector(17 downto 0) := "000000110111110011";
	
	-- The value 7142 which is approx 50Mhz/7Khz.
	constant RESET_SOUND_EVERY_7KHZ : std_logic_vector(17 downto 0) := "000001101111100110";
	
	-- The value 8333 which is approx 50Mhz/(4Khz * 2).
	constant CHANGE_SOUND_EVERY_4KHZ : std_logic_vector(17 downto 0) := "000010000010001101";
	
	-- The value 16666 which is approx 50Mhz/4Khz.
	constant RESET_SOUND_EVERY_4KHZ : std_logic_vector(17 downto 0) := "000100000100011010";
	
	-- The value 25000 which is exactly 50Mhz/(1Khz * 2).
	constant CHANGE_SOUND_EVERY_1KHZ : std_logic_vector(17 downto 0) := "000110000110101000";
	
	-- The value 50000 which is exactly 50Mhz/1Khz.
	constant RESET_SOUND_EVERY_1KHZ : std_logic_vector(17 downto 0) := "001100001101010000";
	
	-- The value 2500 which is exactly 50Mhz/(10Khz * 2).
	constant CHANGE_SOUND_EVERY_10KHZ : std_logic_vector(17 downto 0) := "000000100111000100";
	
	-- The value 5000 which is exactly 50Mhz/10Khz.
	constant RESET_SOUND_EVERY_10KHZ : std_logic_vector(17 downto 0) := "000001001110001000";
	
begin
	process(clk_50, counter, buttonPressed)
		begin
			if(rising_edge(clk_50)) then
				counter <= std_logic_vector(unsigned(counter) + 1);
				if buttonPressed(0) = '1' then
					if counter >= CHANGE_SOUND_EVERY_400HZ then
							sound <= '1';
						if counter >= RESET_SOUND_EVERY_400HZ then
							counter <= "000000000000000000";
						end if;
					else
						sound	<= '0';
					end if;
				elsif buttonPressed(1) = '1' then
					if counter >= CHANGE_SOUND_EVERY_7KHZ then
							sound <= '1';
						if counter >= RESET_SOUND_EVERY_7KHZ then
							counter <= "000000000000000000";
						end if;
					else
						sound <= '0';
					end if;
				elsif buttonPressed(2) = '1' then
					if counter >= CHANGE_SOUND_EVERY_4KHZ then
						sound <= '1';
						if counter >= RESET_SOUND_EVERY_4KHZ then
							counter <= "000000000000000000";
						end if;
					else
						sound <= '0';
					end if;
				elsif buttonPressed(3) = '1' then
					if counter >= CHANGE_SOUND_EVERY_1KHZ then
						sound <= '1';
						if counter >= RESET_SOUND_EVERY_1KHZ then
							counter <= "000000000000000000";
						end if;
					else
						sound <= '0';
					end if;
				elsif buttonPressed(4) = '1' then
					if counter >= CHANGE_SOUND_EVERY_10KHZ then
						sound <= '1';
						if counter >= RESET_SOUND_EVERY_10KHZ then
							counter <= "000000000000000000";
						end if;
					else
						sound <= '0';
					end if;
				end if;
			end if;
		end process;
end behavior;