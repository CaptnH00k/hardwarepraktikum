-- ADC reader: reads data from ADXL345
-- Descriptions for the Bitstrings for init0-init4:

-- init0: The address is given by "00110001" which is equal to 49 or rather the address 0x31.
-- The register specified at address 0x31 is the DATA_FORMAT register. We write the following data bitstring
-- "01001000". The '1' at index 6 is a flag which is called the SPI bit, which sets the device
-- to a 3-wire SPI mode, which makes sense since we're using a 3-wire SPI mode. The '1' at index 3 is the so called
-- FULL_RES bit which sets this device into "full resolution" mode which makes the output more accurate which can further
-- be enhanced by the Range bits of this register.

-- init1: The address is given by "00100100" which is equal to 36 or rather the address 0x24.
-- The register specified at address 0x24 is the THRESH_ACT register. We write the following data bitstring
-- "00000010". Here are no flags to set, the bitstring is interpreted as 2 since "10" = 2, and it sets
-- the activity threshold for this device to that value. Meaning there's an activity flag set to true
-- internally if the current activity is > 2.

-- init2: The address is given by "00101100" which is equal to 44 or rather the address 0x2C.
-- The register specified at the address 0x2C is the BW_RATE register. We write the following data bitstring
-- "00001001". The first 4 bits here are interpreted as 9 since "1001" = 9, and it sets the output data rate
-- this value. Since the default value is 10 and translates to 100Hz, I'd assume 9 translates to 90Hz.

-- init3: The address is given by "00101110" which is equal to 46 or rather the address 0x2E.
-- The register specified at the address 0x2E is the INT_ENABLE register. We write the following data bitstring
-- "00010000". The bit at index 4 is the Activity bit, which when set to 1 will allow activity interrupts on this
-- device, which is self explanatory.

-- init4: The address is given by "00101101" which is equal to 45 or rather the address 0x2D.
-- The register specified at the address 0x2D is the POWER_CTL register. We write the following data bitstring
-- "00001000". The bit at index 3 is the so called Measure bit, which when set to 1 will turn this device into
-- measurement mode, whereas turning it to 0 results in the device being in standby mode, so with a 1 set 
-- the device won't be powered by minimum power.


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity g_reader is port (
	clk_50: in std_logic;	-- 50 MHz clock
	
	reset_n : in std_logic; -- reset signal (active low)

	-- SPI interface
	CS_N : out std_logic;   -- connected to chip select of g sensor
	SCLK : out std_logic;   -- spi clock
	SDIO : inout std_logic; -- spi data (bidirectional)
		
	-- data output
	dataX : out std_logic_vector(12 downto 0);
	dataY : out std_logic_vector(12 downto 0);
	dataZ : out std_logic_vector(12 downto 0)
);
end g_reader;


architecture behavior of g_reader is
	type state_type is (init0, init1, init2, init3, init4, readX, readY, readZ, pause);

	signal state : state_type := pause;
	signal next_state : state_type := init0;
	
	signal SCLK_counter : unsigned(5 downto 0) := (others => '0');
	signal SCLK_int : std_logic := '0';
	
	signal CS_N_int : std_logic := '0';
	
	signal transmission_count : unsigned(4 downto 0) := (others => '0');
	signal time_count : unsigned(15 downto 0) := (others => '0');
	
	signal read_temp : unsigned(12 downto 0) := (others => '0');
	
	constant init0_transmission : unsigned(15 downto 0) := "0011000101001000";
	constant init1_transmission : unsigned(15 downto 0) := "0010010000000010";
	constant init2_transmission : unsigned(15 downto 0) := "0010110000001001";
	constant init3_transmission : unsigned(15 downto 0) := "0010111000010000";
	constant init4_transmission : unsigned(15 downto 0) := "0010110100001000";
	
	constant readX_transmission : unsigned(7 downto 0) := "11110010";
	constant readY_transmission : unsigned(7 downto 0) := "11110100";
	constant readZ_transmission : unsigned(7 downto 0) := "11110110";
 	
begin
	process(clk_50)
		begin
			if (rising_edge(clk_50)) then
				 if SCLK_counter = 24 then
					if SCLK_int = '0' then
						SCLK_int <= '1';
					elsif SCLK_int = '1' then
						SCLK_int <= '0';
					end if;
					
					SCLK_counter <= (others => '0');
					
				 else
					SCLK_counter <= SCLK_counter + 1;
					
				end if;
			end if;
		end process;
	
		process(clk_50, reset_n)
			begin
				if (rising_edge(clk_50)) then
				
					if reset_n = '0' then
						state <= pause;
						next_state <= init0;
						
					end if;
				
					if state = init0 then
						if transmission_count = 16 then
							time_count <= (others => '0');
							state <= pause;
							next_state <= init1;
						end if;
					
					elsif state = init1 then
						if transmission_count = 16 then
							time_count <= (others => '0');
							state <= pause;
							next_state <= init2;
						end if;
						
					elsif state = init2 then
						if transmission_count = 16 then
							time_count <= (others => '0');
							state <= pause;
							next_state <= init3;
						end if;
						
					elsif state = init3 then
						if transmission_count = 16 then
							time_count <= (others => '0');
							state <= pause;
							next_state <= init4;
						end if;
						
					elsif state = init4 then
						if transmission_count = 16 then
							time_count <= (others => '0');
							state <= pause;
							next_state <= readX;
						end if;
						
					elsif state = pause then
						time_count <= time_count + 1;
						
						if next_state = readX and time_count >= 50000 and SCLK_int = '1' then
							state <= readX;
							
						elsif next_state /= readX and time_count >= 500 and SCLK_int = '1' then
							state <= next_state;
						
						end if;
					
					elsif state = readX then
						if transmission_count = 25 then
							time_count <= (others => '0');
							state <= pause;
							next_state <= readY;
						end if;
						
					elsif state = readY then
						if transmission_count = 25 then
							time_count <= (others => '0');
							state <= pause;
							next_state <= readZ;
						end if;
						
					elsif state = readZ then
						if transmission_count = 25 then
							time_count <= (others => '0');
							state <= pause;
							next_state <= readX;
						end if;
					end if;
				end if;
			end process;
			
			process(SCLK_int)
				begin
					if falling_edge(SCLK_int) then
						if CS_N_int = '0' then
							if state = init0 then
								SDIO <= init0_transmission(to_integer(15 - transmission_count));
								
							elsif state = init1 then
								SDIO <= init1_transmission(to_integer(15 - transmission_count));
								
							elsif state = init2 then
								SDIO <= init2_transmission(to_integer(15 - transmission_count));
							
							elsif state = init3 then
								SDIO <= init3_transmission(to_integer(15 - transmission_count));
							
							elsif state = init4 then
								SDIO <= init4_transmission(to_integer(15 - transmission_count));
							
							elsif state = readX then			
								if transmission_count < 8 then
									SDIO <= readX_transmission(to_integer(7 - transmission_count));
																
								elsif transmission_count = 8 then
									SDIO <= 'Z';
								end if;
							
							elsif state = readY then
								if transmission_count < 8 then
									SDIO <= readY_transmission(to_integer(7 - transmission_count));
																
								elsif transmission_count = 8 then
									SDIO <= 'Z';
								end if;
							
							elsif state = readZ then
								if transmission_count < 8 then
									SDIO <= readZ_transmission(to_integer(7 - transmission_count));
																
								elsif transmission_count = 8 then
									SDIO <= 'Z';
								end if;
							end if;
						else
							SDIO <= 'Z';
							
						end if;
					end if;
				end process;
				
				process(SCLK_int)
					begin
						if rising_edge(SCLK_int) then
							if CS_N_int = '0' then
								transmission_count <= transmission_count + 1;
								
							else
								transmission_count <= (others => '0');
								read_temp <= (others => '0');
							end if;
							
							if transmission_count >= 8 and transmission_count < 16 then
								read_temp(to_integer(15 - transmission_count)) <= SDIO;
								
							elsif transmission_count >= 19 and transmission_count < 24 then
								read_temp(to_integer(31 - transmission_count)) <= SDIO;
								
							elsif transmission_count = 24 then
								if state = readX then
									dataX <= std_logic_vector(read_temp);
								elsif state = readY then
									dataY <= std_logic_vector(read_temp);
								elsif state = readZ then
									dataZ <= std_logic_vector(read_temp);
								end if;
							end if;
						end if;
				end process;
					
			
			CS_N <= '1' when state = pause else '0'; -- Since this signal is low active.
			CS_N_int <= '1' when state = pause else '0';
			SCLK <= '1' when state = pause else SCLK_int;
			
end behavior;

