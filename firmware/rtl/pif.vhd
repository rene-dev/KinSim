library IEEE;       use IEEE.std_logic_1164.all;
library machxo2;    use machxo2.components.all;

--=====================================================================
entity pif is
   port (
	 	GSRn      : in  std_logic;
    LEDR      : out std_logic;
    LEDG      : out std_logic;

		-- I2C
		J1_3      : in std_logic; -- SDA
		J1_5      : in std_logic; -- SCL

		-- SPI
		J1_19      : in std_logic; -- MOSI -- bit 3
		J1_21      : in std_logic; -- MISO -- bit 2
		J1_23      : in std_logic; -- SCLK -- bit 4
		J1_24      : in std_logic; -- CE0 -- bit 1
		J1_26      : in std_logic; -- CE0 -- bit 0

		-- J1
		J1_7       : in std_logic; -- bit 5
		J1_8       : in std_logic;
		J1_10       : in std_logic;
		J1_11       : in std_logic; -- bit 6
		J1_12       : in std_logic;
		J1_13       : in std_logic;
		J1_15       : in std_logic; -- bit 7
		J1_16       : in std_logic; -- bit rw
		J1_18       : in std_logic; -- bit strobe
		J1_22       : in std_logic;

		-- J2
		J2_7       : in std_logic;
		J2_8       : in std_logic;
		J2_10       : in std_logic;
		J2_11       : in std_logic;
		J2_12       : in std_logic;
		J2_13       : in std_logic;
		J2_15       : in std_logic;
		J2_16       : in std_logic;
		J2_18       : in std_logic;
		J2_22       : in std_logic;
		J2_24       : in std_logic;
		J2_26       : in std_logic;

		-- J3
		J3_1       : in std_logic;
		J3_2       : in std_logic;
		J3_3       : in std_logic;
		J3_4       : in std_logic;
		J3_6       : in std_logic;
		J3_7       : in std_logic;
		J3_8       : in std_logic;

		-- J4
		J4_1       : in std_logic;
		J4_2       : in std_logic;
		J4_3       : in std_logic;
		J4_4       : in std_logic;
		J4_7       : in std_logic;
		J4_8       : in std_logic;
		J4_9       : in std_logic;
		J4_10      : in std_logic;
		J4_12       : in std_logic;
		J4_13       : in std_logic
	);
end pif;

--=====================================================================
architecture rtl of pif is
	component osch is
		-- synthesis translate_off
		generic (nom_freq: string := "2.08");
		-- synthesis translate_on
		port(
			stdby    :in  std_logic;
      osc      :out std_logic;
			sedstdby :out std_logic
		);
	end component osch;

	signal clk 		: std_logic;

	constant OSC_STR  : string  := "44.33";
	attribute nom_freq : string;
	attribute nom_freq of oscinst0 : label is OSC_STR;
    
    signal data : std_logic_vector(7 downto 0);

begin
	OSCInst0: osch
    -- synthesis translate_off
    generic map ( nom_freq => OSC_STR )
    -- synthesis translate_on
    port map(
			stdby    => '0',
      osc      => clk,
			sedstdby => open
		);
    
    data <= (J1_15, J1_11, J1_7, J1_23, J1_19, J1_21, J1_24, J1_26);
        
        

	pwm1:entity pwm port map(clk,data,LEDR);
	pwm2:entity pwm port map(clk,30,LEDG);


end rtl;
