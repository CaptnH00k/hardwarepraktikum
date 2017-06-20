-- controller: controles 3 ultrasound sensors
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity controller is port(
	clk_50 : in std_logic;
	
	finished_1 : in std_logic;
	finished_2 : in std_logic;
	finished_3 : in std_logic;
	
	enable_1 : out std_logic;
	enable_2 : out std_logic;
	enable_3 : out std_logic
);
end controller;


architecture behavior of controller is
	-- The states used throughout this FSM
	type state_type is (us1, us2, us3);
	
	-- A flag determining whether the US is busy or not.
	signal busy : std_logic := '0';
	
	-- The current state of the FSM
	signal state : state_type := us1;
		
begin
	process(clk_50)
	begin
		if rising_edge(clk_50) then
			
			if state = us1 then            
				-- Intuitive: if the first US is ready but isn't busy let it measure distance.
				if finished_1 = '1' and busy = '0' then
					enable_1 <= '1';
					enable_2 <= '0';
					enable_3 <= '0';
					
				-- now the US is measuring distance, so we don't have to send enable signals anymore, and can set our busy flag.
				elsif finished_1 = '0' and busy = '0' then
					enable_1 <= '0';
					busy <= '1';
					
				-- now the US is finished with its measurements, thus we can switch to the next US and reset the busy flag.
				elsif finished_1 = '1' and busy = '1' then
					busy <= '0';
					state <= us2;
					
				end if;

			elsif state = us2 then
				-- See US1 for explanations.
				if finished_2 = '1' and busy = '0' then
					enable_1 <= '0';
					enable_2 <= '1';
					enable_3 <= '0';
					
				elsif finished_2 = '0' and busy = '0' then
					enable_2 <= '0';
					busy <= '1';
					
				elsif finished_2 = '1' and busy = '1' then
					busy <= '0';
					state <= us3;
					
				end if;
				
			elsif state = us3 then
				-- See US1 for explanations.
				if finished_3 = '1' and busy = '0' then
					enable_1 <= '0';
					enable_2 <= '0';
					enable_3 <= '1';
					
				elsif finished_3 = '0' and busy = '0' then
					enable_3 <= '0';
					busy <= '1';
					
				elsif finished_3 = '1' and busy = '1' then
					busy <= '0';
					state <= us1;
					
				end if;
			end if;
		end if;
	end process;
end behavior;