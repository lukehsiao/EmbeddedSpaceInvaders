----------------------------------------------------------------------------------
-- Luke Hsiao & Jeff Ravert
-- Brigham Young University
-- ECEn 427 | Lab 7
-- Ultrasonic Sensor Hardware Module
--
-- Description:
--   This module constantly pings Trigger, and listens to echo using the
--   HC-SR04 Ultrasonic Sensor, then stores the raw echo length in a 
--   register to be processed by the software.
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity ultrasonic_hw is
    Port ( clk : in  STD_LOGIC;
           echo : in  STD_LOGIC;
           trigger : out  STD_LOGIC;
           distance_reg : out  STD_LOGIC_VECTOR (31 downto 0));
end ultrasonic_hw;

architecture Behavioral of ultrasonic_hw is

constant ZEROS: unsigned(31 downto 0) := (others => '0');
constant TRIGGER_DELAY: integer := 1010;
constant STORE_DELAY: integer := 300000; -- delay of 5ms on a 10ns period clk
constant RETRIGGER_DELAY: integer := 6000000; -- delay of 100ms in a 10ns clk

-- Used to send a 100us Trigger Signal
signal triggerCounter, triggerCounter_next: unsigned(31 downto 0) := ZEROS;

-- Increments while ECHO is high.
signal echoCounter, echoCounter_next: unsigned(31 downto 0) := ZEROS;

signal idleCounter, idleCounter_next: unsigned(31 downto 0):= ZEROS;

-- Used to delay between echo falling and triggering again
signal storeCounter, storeCounter_next: unsigned(31 downto 0) := ZEROS;

-- Output Register
signal distance_reg_i, distance_reg_i_next: std_logic_vector(31 downto 0) := (others => '0');

-- FSM Signals
type state is (TRGR, IDLE, LISTEN, STORE);
signal state_cs, state_ns: state := TRGR;

begin

    -- State Machine + Counters
    process(clk)
    begin
        if (clk'event and clk = '1') then
            state_cs <= state_ns;
            triggerCounter <= triggerCounter_next;
            echoCounter <= echoCounter_next;
            storeCounter <= storeCounter_next;
				idleCounter <= idleCounter_next;
            distance_reg_i <= distance_reg_i_next;
        end if;
    end process;

    -- Next State Logic
    process(state_cs, triggerCounter, echo, echoCounter, storeCounter, distance_reg_i, idleCounter)
    begin
        -- default
        state_ns <= state_cs;
        triggerCounter_next <= triggerCounter;
        echoCounter_next <= echoCounter;
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
                echoCounter_next <= ZEROS;
                if (echo = '1') then
                    state_ns <= LISTEN;
                end if;
					 if (idleCounter = RETRIGGER_DELAY) then
					     state_ns <= TRGR;
						  triggerCounter_next <= ZEROS;
						  echoCounter_next <= (others => '1');
					 end if;
            when LISTEN =>
                echoCounter_next <= echoCounter + 1;
                if (echo = '0') then
                    state_ns <= STORE;
                end if;
            when others =>	-- when STORE
                distance_reg_i_next <= std_logic_vector(echoCounter);
                triggerCounter_next <= ZEROS;
                storeCounter_next <= storeCounter + 1;
                if (storeCounter = STORE_DELAY) then
                    state_ns <= TRGR;
                    storeCounter_next <= ZEROS;
                end if;
        end case;
    end process;


    -- Output Forming Logic
	 distance_reg <= distance_reg_i;
    process(state_cs, triggerCounter, echo)
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

end Behavioral;

