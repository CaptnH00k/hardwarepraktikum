library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity headingBuffer is port(
heading_in : in std_logic_vector(7 downto 0);
dataReady : in std_logic;

heading_out : out std_logic_vector(8 downto 0)

);
end headingBuffer;

architecture behavior of headingBuffer is

begin
	process(dataReady)
		begin
			if(rising_edge(dataReady)) then
				heading_out <= heading_in & '0';
				
			end if;
			
	end process;

end behavior;