library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity myCounter is port(
	count 	: in std_logic; -- count signal: increase counter on rising edge
	rst 		: in std_logic; -- reset signal: set counter to 0 if rst = 0
	countOut : out std_logic_vector(2 downto 0)); -- counter output
end myCounter;

architecture behavioral of myCounter is
	signal temp_count : std_logic_vector(2 downto 0); -- The internal counter used to count up
begin
	process(count, rst)
		begin
			if rst = '0' then
				temp_count <= "000";
			elsif (rising_edge(count)) then
			-- No need to check for overflow since the std_logic_vector type already handles it appropriatly.
				temp_count <= std_logic_vector(unsigned(temp_count) + 1);
			end if;
		end process;
		countOut <= temp_count;
end behavioral;