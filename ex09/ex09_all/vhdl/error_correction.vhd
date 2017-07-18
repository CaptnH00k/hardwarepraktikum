library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity error_correction is port(
	clk_50 : in std_logic;
	
	dataReady : out std_logic;
	
	--block_index : out std_logic_vector(4 downto 0);
	--paritybit_check : out std_logic;
	--current_parity_bit : out std_logic_vector(5 downto 0);
	--vector_parity : out std_logic_vector(5 downto 0);
	--bits_error : out std_logic_vector(5 downto 0);
	corrected_vector : out std_logic_vector(31 downto 0)
	
);
end error_correction;

architecture behavior of error_correction is
	
	-- the states used for this module.
	type state_type is (check_error, correct_error, initiate, apply_changes);
	
	-- the array which holds the uncorrected vectors.
	type dataElement is array(0 to 28) of unsigned(37 downto 0);
	
	-- the current state
	signal state : state_type := initiate;
	
	-- signals whether there are more vectors to check or not.
	signal finished : std_logic := '0';
	
	-- the index of the current block being corrected.
	signal current_block_index : unsigned(4 downto 0) := (others => '0');
	
	-- a vector holding only the parity values of the not corrected current vector.
	signal parity_vector : unsigned(5 downto 0) := (others => '0');
	
	-- the own calculates parity bit to check with the actualy parity bit.
	signal current_paritybit_check : std_logic := '0';
	
	-- a counter used to iterate through the vectors bits.
	signal data_counter : unsigned(5 downto 0) := (others => '0');
	
	-- a vector which holds the value of where the error is in the current vector.
	signal error_bits : unsigned(5 downto 0) := (others => '0');
	
	-- a number ranging from 0 to 6 which signals which parity bit is used at the moment.
	signal current_paritybit_position : unsigned(5 downto 0) := (others => '0');
	
	-- a vector which is the corrected version of the actual current vector.
	signal corrected_data : unsigned(37 downto 0) := (others => '0');
	
	-- the constant values given.
	constant uncheckedData : dataElement := (
		"10111111001110000110101001110101001111", 
		"11000000011111100101110010011101111110", 
		"10000101110101110110000101110110110101", 
		"11000000011111100110110000110100001000", 
		"10000101110101110110100110101111100010", 
		"11000000011111110111111000101110010001", 
		"01110101110100100000001110000100111100", 
		"01111111011111110001100010000001000111", 
		"11110110110010110110100000010010010000", 
		"10000101110101110101111110001101111110", 
		"01000101010111110001101010010011000110", 
		"01001001111100111101101010000100001101", 
		"10000101110101110101100011010101111011", 
		"01111111000010111001101010000001001110", 
		"10111111111011101101000000100110010110", 
		"10010110011001001110110001100101010101", 
		"10000101110101110110111110011101110111", 
		"11000000011111100111111101010100011101", 
		"10000101110101110110111110000000111101", 
		"01000101001111100101001011100111000100", 
		"11000000011111100111111000011001001001", 
		"10000101110101110111110101110111111011", 
		"11000000011111100101110000100100110110", 
		"10110010111100000000000110000110111101", 
		"10110011001011011101110001001001010111", 
		"10010111000001100001101010000001000101", 
		"01111000000000101101110101100011010110", 
		"10000101110101110101001110001100011100", 
		"10111111000110010001100110110101000010");

begin
	process(clk_50)
		begin
			if(rising_edge(clk_50)) then
				if state = initiate and finished = '0' then
					parity_vector(0) <= uncheckedData(to_integer(current_block_index))(0);
					parity_vector(1) <= uncheckedData(to_integer(current_block_index))(1);
					parity_vector(2) <= uncheckedData(to_integer(current_block_index))(3);
					parity_vector(3) <= uncheckedData(to_integer(current_block_index))(7);	
					parity_vector(4) <= uncheckedData(to_integer(current_block_index))(15);
					parity_vector(5) <= uncheckedData(to_integer(current_block_index))(31);
					corrected_data <= uncheckedData(to_integer(current_block_index));
					
					current_paritybit_position <= (others => '0');						
					current_paritybit_check <= '0';
					data_counter <= (others => '0');
					error_bits <= (others => '0');
					
					state <= check_error;
					
				elsif state = check_error then
					if data_counter >= 38 then
						
						--block_index <= std_logic_vector(current_block_index);
						--paritybit_check <= current_paritybit_check;
						--current_parity_bit <= std_logic_vector(current_paritybit_position);
						--vector_parity <= std_logic_vector(parity_vector);
						--bits_error <= std_logic_vector(error_bits);
						
						if current_paritybit_position < 6 and current_paritybit_check /= parity_vector(to_integer(current_paritybit_position)) then
							error_bits(to_integer(current_paritybit_position)) <= '1';
						
						end if;
						
						if current_paritybit_position = 6 then
							if error_bits = 0 then
								state <= apply_changes;
								
							else
								state <= correct_error;
								
							end if;
							
						else
							current_paritybit_position <= current_paritybit_position + 1;
						
						end if;
						
						current_paritybit_check <= '0';
						data_counter <= (others => '0');
						
						
					elsif current_paritybit_position < 6 and data_counter + 1 /= 4 and data_counter + 1 /= 1 and data_counter + 1 /= 2 and data_counter + 1 /= 8 and data_counter + 1 /= 16 and data_counter + 1 /= 32 then
						if data_counter(to_integer(current_paritybit_position)) = '1' then
							current_paritybit_check <= current_paritybit_check xor uncheckedData(to_integer(current_block_index))(to_integer(data_counter));
							
						end if;
						data_counter <= data_counter + 1;
					else
						data_counter <= data_counter + 1;
					end if;
					
				elsif state = correct_error then
						-- flip the bit at the position where the error occured.
						corrected_data(to_integer(error_bits)) <= not corrected_data(to_integer(error_bits));
			
						state <= apply_changes;
						
				elsif state = apply_changes then
						corrected_vector(31 downto 26) <= std_logic_vector(corrected_data(37 downto 32));
						corrected_vector(25 downto 11) <= std_logic_vector(corrected_data(30 downto 16));
						corrected_vector(10 downto 4) <= std_logic_vector(corrected_data(14 downto 8));
						corrected_vector(3 downto 1) <= std_logic_vector(corrected_data(6 downto 4));
						corrected_vector(0) <= corrected_data(2);
						
						-- TODO sent impulses to the next module so it can fetch its correct data. Implement a little dataready clock, with short rising edges so the module can snatch the data.
						-- maybe implement a new state "send data" to ensure a pulse.
						
						if current_block_index >= 28 then
							finished <= '1';
							
						else
							current_block_index <= current_block_index + 1;
							
						end if;
						
						state <= initiate;
				end if;
			end if;
	end process;
	
	dataReady <= finished;
	
end behavior;