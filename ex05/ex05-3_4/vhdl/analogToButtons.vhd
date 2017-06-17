-- analogToButtons
-- converts the reading of the adc into the corresponding button


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity analogToButtons is port(
	analogIn : in std_logic_vector(11 downto 0);
	
	buttonPressed : out std_logic_vector(4 downto 0)
);
end analogToButtons;


architecture behavior of analogToButtons is
	constant FIRST_BUTTON_THRESHOLD : std_logic_vector(11 downto 0) := "001001011000"; -- The value 600
	constant SECOND_BUTTON_THRESHOLD : std_logic_vector(11 downto 0) := "010101111000"; -- The value 1400
	constant THIRD_BUTTON_THRESHOLD : std_logic_vector(11 downto 0) := "011111010000"; -- The value 2000
	constant FOURTH_BUTTON_THRESHOLD : std_logic_vector(11 downto 0) := "101110111000"; -- The value 3000
	constant FIFTH_BUTTON_THRESHOLD : std_logic_vector(11 downto 0) := "111000010000"; -- The value 3600
begin
	process(analogIn)
		begin
			if analogIn < FIRST_BUTTON_THRESHOLD then
				buttonPressed <= "00001";
			elsif analogIn < SECOND_BUTTON_THRESHOLD then
				buttonPressed <= "00010";
			elsif analogIn < THIRD_BUTTON_THRESHOLD then
				buttonPressed <= "00100";
			elsif analogIn < FOURTH_BUTTON_THRESHOLD then
				buttonPressed <= "01000";
			elsif analogIn < FIFTH_BUTTON_THRESHOLD then
				buttonPressed <= "10000";
			else
				buttonPressed <= "00000";
			end if;
		end process;
end behavior;