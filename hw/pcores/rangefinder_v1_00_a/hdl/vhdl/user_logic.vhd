------------------------------------------------------------------------------
-- user_logic.vhd - entity/architecture pair
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          user_logic.vhd
-- Version:           1.00.a
-- Description:       User logic.
-- Date:              Sat Nov 15 15:14:11 2014 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_NUM_REG                    -- Number of software accessible registers
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Resetn                -- Bus to IP reset
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_NUM_REG                      : integer              := 4;
    C_SLV_DWIDTH                   : integer              := 32-- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    --USER ports added here
	 photoIn : in  STD_LOGIC;
    echoIn : in  STD_LOGIC;
    trigger : out  STD_LOGIC;
    
    SW0 : in  STD_LOGIC;
    SW1 : in  STD_LOGIC;
    SW2 : in  STD_LOGIC;
    SW3 : in  STD_LOGIC;
    SW4 : in  STD_LOGIC;
    SW5 : in  STD_LOGIC;
    SW6 : in  STD_LOGIC;
    SW7 : in  STD_LOGIC;
    
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Resetn                  : in  std_logic;
    Bus2IP_Data                    : in  std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    Bus2IP_BE                      : in  std_logic_vector(C_SLV_DWIDTH/8-1 downto 0);
    Bus2IP_RdCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    Bus2IP_WrCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    IP2Bus_Data                    : out std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT : string;
  attribute SIGIS : string;

  attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
  attribute SIGIS of Bus2IP_Resetn : signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is

  --USER signal declarations added here, as needed for user logic

    constant ZEROS: unsigned(31 downto 0) := (others => '0');
    constant TRIGGER_DELAY: integer := 1010;
    constant STORE_DELAY: integer := 300000; -- delay of 3ms on a 10ns period clk
	 constant RETRIGGER_DELAY: integer := 6000000; -- delay of 100ms in a 10ns clk

    -- Used to send a 100us Trigger Signal
    signal triggerCounter, triggerCounter_next: unsigned(31 downto 0) := ZEROS;

    -- Increments while echoIn is high.
    signal echoInCounter, echoInCounter_next: unsigned(31 downto 0) := ZEROS;
	 
	 -- Counter to retrigger if no echo comes.
	 signal idleCounter, idleCounter_next: unsigned(31 downto 0):= ZEROS;

    -- Used to delay between echoIn falling and triggering again
    signal storeCounter, storeCounter_next: unsigned(31 downto 0) := ZEROS;
	 
	 signal photo, photo_next: std_logic;
	 signal photo1, photo_next1: std_logic;
	 signal photo2, photo_next2: std_logic;

    -- Output Register
    signal distance_reg_i, distance_reg_i_next: std_logic_vector(31 downto 0) := x"12345678";

    -- FSM Signals
    type state is (TRGR, IDLE, LISTEN, STORE);
    signal state_cs, state_ns: state := TRGR;
	 --signal state_decode: std_logic_vector(31 downto 0);
    signal clk : std_logic;

  ------------------------------------------
  -- Signals for user logic slave model s/w accessible register example
  ------------------------------------------
  signal slv_reg0                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_reg1                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_reg2                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_reg3                       : std_logic_vector(C_SLV_DWIDTH-1 downto 0); -- used for switches
  signal slv_reg_write_sel              : std_logic_vector(3 downto 0);
  signal slv_reg_read_sel               : std_logic_vector(3 downto 0);
  signal slv_ip2bus_data                : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_read_ack                   : std_logic;
  signal slv_write_ack                  : std_logic;
  signal dummy                          : std_logic_vector(23 downto 0);
  signal sw_buffer                      : std_logic_vector(7 downto 0);

begin
dummy <= (others => '0');

  --USER logic implementation added here

  ------------------------------------------
  -- Example code to read/write user logic slave model s/w accessible registers
  -- 
  -- Note:
  -- The example code presented here is to show you one way of reading/writing
  -- software accessible registers implemented in the user logic slave model.
  -- Each bit of the Bus2IP_WrCE/Bus2IP_RdCE signals is configured to correspond
  -- to one software accessible register by the top level template. For example,
  -- if you have four 32 bit software accessible registers in the user logic,
  -- you are basically operating on the following memory mapped registers:
  -- 
  --    Bus2IP_WrCE/Bus2IP_RdCE   Memory Mapped Register
  --                     "1000"   C_BASEADDR + 0x0
  --                     "0100"   C_BASEADDR + 0x4
  --                     "0010"   C_BASEADDR + 0x8
  --                     "0001"   C_BASEADDR + 0xC
  -- 
  ------------------------------------------
  slv_reg_write_sel <= Bus2IP_WrCE(3 downto 0);
  slv_reg_read_sel  <= Bus2IP_RdCE(3 downto 0);
  slv_write_ack     <= Bus2IP_WrCE(0) or Bus2IP_WrCE(1) or Bus2IP_WrCE(2) or Bus2IP_WrCE(3);
  slv_read_ack      <= Bus2IP_RdCE(0) or Bus2IP_RdCE(1) or Bus2IP_RdCE(2) or Bus2IP_RdCE(3);

  -- implement slave model software accessible register(s)
  SLAVE_REG_WRITE_PROC : process( Bus2IP_Clk ) is
  begin

    if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
      if Bus2IP_Resetn = '0' then
        --slv_reg0 <= (others => '0'); --reg0 is READ_ONLY
        --slv_reg1 <= (others => '0');
        slv_reg2 <= (others => '0');
        --slv_reg3 <= (others => '0');
      else
        case slv_reg_write_sel is
          when "1000" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                --slv_reg0(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
              end if;
            end loop;
          when "0100" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                --slv_reg1(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
              end if;
            end loop;
          when "0010" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                slv_reg2(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
              end if;
            end loop;
          when "0001" =>
            for byte_index in 0 to (C_SLV_DWIDTH/8)-1 loop
              if ( Bus2IP_BE(byte_index) = '1' ) then
                --slv_reg3(byte_index*8+7 downto byte_index*8) <= Bus2IP_Data(byte_index*8+7 downto byte_index*8);
              end if;
            end loop;
          when others => null;
        end case;
      end if;
    end if;

  end process SLAVE_REG_WRITE_PROC;

  -- implement slave model software accessible register(s) read mux
  SLAVE_REG_READ_PROC : process( slv_reg_read_sel, slv_reg0, slv_reg1, slv_reg2, slv_reg3 ) is
  begin

    case slv_reg_read_sel is
      when "1000" => slv_ip2bus_data <= slv_reg0;
      when "0100" => slv_ip2bus_data <= slv_reg1;
      when "0010" => slv_ip2bus_data <= slv_reg2;
      when "0001" => slv_ip2bus_data <= slv_reg3;
      when others => slv_ip2bus_data <= (others => '0');
    end case;

  end process SLAVE_REG_READ_PROC;

  ------------------------------------------
  -- Example code to drive IP to Bus signals
  ------------------------------------------
  IP2Bus_Data  <= slv_ip2bus_data when slv_read_ack = '1' else
                  (others => '0');

  IP2Bus_WrAck <= slv_write_ack;
  IP2Bus_RdAck <= slv_read_ack;
  IP2Bus_Error <= '0';

  ---------------------------------------------------------------------------------------------------------
  ---------------------------------------------------------------------------------------------------------
  clk <= Bus2IP_Clk;
  
  
    -- State Machine + Counters
    process(clk)
    begin
        if (clk'event and clk = '1') then
            state_cs <= state_ns;
            triggerCounter <= triggerCounter_next;
            echoInCounter <= echoInCounter_next;
            storeCounter <= storeCounter_next;
				idleCounter <= idleCounter_next;
            distance_reg_i <= distance_reg_i_next;
				photo <= photo_next;
				photo1 <= photo_next1;
				photo2 <= photo_next2;
        end if;
    end process;

    -- Next State Logic
    process(state_cs, triggerCounter, echoIn, echoInCounter, storeCounter, distance_reg_i, idleCounter)
    begin
        -- default
        state_ns <= state_cs;
        triggerCounter_next <= triggerCounter;
        echoInCounter_next <= echoInCounter;
        distance_reg_i_next <= distance_reg_i;
		  idleCounter_next <= idleCounter;
        case state_cs is
            when TRGR =>
                triggerCounter_next <= triggerCounter + 1;
                if (triggerCounter = TRIGGER_DELAY) then
                    state_ns <= IDLE;
						  idleCounter_next <= ZEROS;
                    triggerCounter_next <= ZEROS;
                else
                end if;
            when IDLE =>
					 idleCounter_next <= idleCounter + 1;
                echoInCounter_next <= ZEROS;
                if (echoIn = '1') then
                    state_ns <= LISTEN;
                end if;
					 if (idleCounter = RETRIGGER_DELAY) then
					     state_ns <= TRGR;
						  triggerCounter_next <= ZEROS;
						  echoInCounter_next <= (others => '1');
					 end if;
            when LISTEN =>
                echoInCounter_next <= echoInCounter + 1;
                if (echoIn = '0') then
                    state_ns <= STORE;
                end if;
            when others =>	-- when STORE
                distance_reg_i_next <= std_logic_vector(echoInCounter);
                triggerCounter_next <= ZEROS;
                storeCounter_next <= storeCounter + 1;
                if (storeCounter = STORE_DELAY) then
                    state_ns <= TRGR;
                    storeCounter_next <= ZEROS;
                end if;
        end case;
    end process;


    -- Output Forming Logic
	 slv_reg0 <= distance_reg_i;
	 photo_next2 <= photoIn;
	 photo_next1 <= photo2;
	 photo_next <= photo1;
	 slv_reg1 <= (others => photo);
    process(state_cs, triggerCounter, echoIn)
    begin
        -- default
        trigger <= '0';
        case state_cs is
            when TRGR =>
                trigger <= '1';
            when IDLE =>
            when LISTEN =>
            when others =>
        end case;
    end process;
	 
--	 -- Have Reg 2 show system state
--	 process(state_cs)
--	 begin
--		 case state_cs is
--				when TRGR =>
--					state_decode <= x"F0000000";
--				when IDLE =>
--					state_decode <= x"0F000000";
--				when LISTEN =>
--					state_decode <= x"00F00000";
--				when others =>
--					state_decode <= x"000F0000";
--		  end case;	 
--	 end process;
--	 slv_reg2 <= state_decode;
    sw_buffer <= SW7 & SW6 & SW5 & SW4 & SW3 & SW2 & SW1 & SW0;
	slv_reg3 <= dummy & sw_buffer;  -- send switches out
end IMP;

