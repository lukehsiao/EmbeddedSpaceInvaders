----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:07:58 11/15/2014 
-- Design Name: 
-- Module Name:    RangeFinder - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity RangeFinder is
    Port ( clk : in  STD_LOGIC;
			  rst : in  STD_LOGIC;
           trig : in  STD_LOGIC_VECTOR (31 downto 0);
           echo : in  STD_LOGIC;
           max : in  STD_LOGIC_VECTOR (31 downto 0);
           rangeCount : out  STD_LOGIC_VECTOR (31 downto 0));
end RangeFinder;

architecture Behavioral of RangeFinder is
    type pit_state_type is (COUNTING, INTERRUPT, RELOAD);
    signal pit_state_reg, pit_state_next: pit_state_type := COUNTING;
    signal counter_reg, counter_next: unsigned(31 downto 0) := x"0000FFFF";
    signal delay_reg: unsigned(31 downto 0);
    signal counter_en, interrupt_en, reload_en, force_load: std_logic;
    signal clk, rst: std_logic;
begin

	 process(clk, rst)
    begin
        if(rst = '0') then
            pit_state_reg <= COUNTING;
            counter_reg <= x"FFFFFFFF";
        elsif(clk'event and clk = '1') then
            pit_state_reg <= pit_state_next;
            counter_reg <= counter_next;
        end if;
    end process;
    
    -- Next State Logic
    process(reload_en, pit_state_reg, counter_next)
    begin
        case pit_state_reg is
            when COUNTING =>
                if(counter_next = 0) then
                    pit_state_next <= INTERRUPT;
                else
                    pit_state_next <= COUNTING;
                end if;
            when INTERRUPT =>
                pit_state_next <= RELOAD;
                if(reload_en = '1') then
                    pit_state_next <= COUNTING;
                else
                    pit_state_next <= RELOAD;
                end if;
            when RELOAD  =>
                if(reload_en = '1') then
                    pit_state_next <= COUNTING;
                else
                    pit_state_next <= RELOAD;
                end if;
        end case;
    end process;
    
    -- Output Logic    
    process(counter_en, reload_en, force_load, pit_state_reg, interrupt_en, counter_reg, delay_reg)
    begin
        intr <= '0'; -- Default Value
        case pit_state_reg is
            when COUNTING =>
					 if (force_load = '1') then
						  counter_next <= delay_reg;
					 else
						 if (counter_en = '1') then
							  counter_next <= counter_reg - 1;
						 else
							  counter_next <= counter_reg;
						 end if;
					 end if;
            when INTERRUPT =>
                intr <= '1' and interrupt_en;
                if(reload_en = '1') then
                    counter_next <= delay_reg;
                end if;
            when RELOAD  =>
                if(reload_en = '1') then
                    counter_next <= delay_reg;
                end if;
        end case;
    end process;
	
end Behavioral;

