library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity UARTreceiver is port(
clk_50 : in std_logic;
dataIn : in std_logic;

dataOut : out std_logic_vector(7 downto 0);
dataReady : out std_logic

);
end UARTreceiver;

architecture behavior of UARTreceiver is
	
	type state_type is (idle, transmission_start, data_transmission, transmission_stop);
	
	signal state : state_type := idle;
	
	signal bitCounter : unsigned (10 downto 0) := (others => '0');
	signal currentBit : unsigned (3 downto 0) := (others => '0');

begin
	process(clk_50, dataIn)
		begin
			if(rising_edge(clk_50)) then
				if state = idle then
					if dataIn = '0' then
						dataReady <= '0';	
						state <= transmission_start;
					end if;
					
				elsif state = transmission_start then
					bitCounter <= bitCounter + 1;
					
					if bitCounter = 1300 then
						bitCounter <= (others => '0');
						state <= data_transmission;
					end if;
					
				elsif state = data_transmission then
					bitCounter <= bitCounter + 1;
					
					if bitCounter = 650 then
						dataOut(to_integer(currentBit)) <= dataIn;
						
					elsif bitCounter = 1300 then
						bitCounter <= (others => '0');
						currentBit <= currentBit + 1;
						
					elsif currentBit > 7 then
						currentBit <= (others => '0');
						bitCounter <= (others => '0');
						state <= transmission_stop;
						
					end if;
					
				elsif state = transmission_stop then
					bitCounter <= bitCounter + 1;
					
					if bitCounter = 650 and dataIn = '1' then
						dataReady <= '1';
					elsif bitCounter = 1300 then
						bitCounter <= (others => '0');
						dataReady <= '0';
						state <= idle;
	
					end if;
				end if;
		end if;
	end process;

end behavior;