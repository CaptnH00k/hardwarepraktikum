library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity decodeData is port(
	key : in std_logic_vector(31 downto 0);
	next_block : in std_logic;
	
	keyOk : out std_logic;
	finished : out std_logic;
	decoded_data : out std_logic_vector(31 downto 0);
	dataReady : out std_logic
);
end decodeData;

architecture behavior of decodeData is

	type state_type is (substitude, shift_rows, add_round_key, check_special_blocks);
	
	type substitudeElement is array(0 to 15) of unsigned(3 downto 0);

	type dataElement is array(0 to 28) of unsigned(31 downto 0);
	
	signal state : state_type := substitude;
	
	signal decoding : std_logic := '0';
	
	signal current_decoded_data : unsigned(31 downto 0) := (others => '0');
	
	signal current_block_index : unsigned(4 downto 0) := (others => '0');
	
	constant initial_block_data : unsigned(31 downto 0) := "11111111000000001111111100000000";
	
	constant last_vector_index : unsigned(4 downto 0) := "11100";

	constant substitudeTable : substitudeElement := (
	"1010", "1100", "1000", "1111", "1110", "0110", "0011", "1011",
	"0111", "0101", "0000", "0010", "1001", "0001", "0100", "1101");

begin
end behavior;