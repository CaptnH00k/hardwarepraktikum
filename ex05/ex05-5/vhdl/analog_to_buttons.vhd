-- analog_to_buttons
-- converts the reading of the adc into the corresponding button


library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity analog_to_buttons is port(
	analogIn : in std_logic_vector(11 downto 0);
	
	button1pressed : out std_logic;
	button3pressed : out std_logic
);
end analog_to_buttons;


architecture behavior of analog_to_buttons is
	constant FIRST_BUTTON_THRESHOLD : std_logic_vector(11 downto 0) := "000000010000"; -- The value 16
	constant SECOND_BUTTON_THRESHOLD : std_logic_vector(11 downto 0) := "010101111000"; -- The value 1400
	constant THIRD_BUTTON_THRESHOLD : std_logic_vector(11 downto 0) := "011111010000"; -- The value 2000
begin
	process(analogIn)
		begin
			if analogIn < FIRST_BUTTON_THRESHOLD then
				button1pressed <= '1';
				button3pressed <= '0';
			elsif ((analogIn > SECOND_BUTTON_THRESHOLD) and (analogIn < THIRD_BUTTON_THRESHOLD)) then
				button3pressed <= '1';
				button1pressed <= '0';
			else
				button1pressed <= '0';
				button3pressed <= '0';
			end if;
		end process;
end behavior;