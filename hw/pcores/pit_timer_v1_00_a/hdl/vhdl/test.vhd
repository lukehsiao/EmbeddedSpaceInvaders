library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity pit_build is
    Port ( clk : in  STD_LOGIC;
           rst : in  STD_LOGIC;
           reg0 : in  STD_LOGIC_VECTOR (31 downto 0);
           reg1 : in  STD_LOGIC_VECTOR (31 downto 0);
           intr : out  STD_LOGIC);
end pit_build;

architecture Behavioral of pit_build is
	type pit_state_type is (COUNTING, INTERRUPT, RELOAD);
	signal pit_state_reg, pit_state_next: pit_state_type := COUNTING;
	signal counter_reg, counter_next: unsigned(31 downto 0) := (others => '1');
	signal delay_reg: unsigned(31 downto 0);
	signal counter_en, interrupt_en, reload_en: std_logic;
begin

	counter_en 		<= reg1(0);
	interrupt_en 	<= reg1(1);
	reload_en 		<= reg1(2);
	
	delay_reg		<= unsigned(reg0);
	
	-- FSM + Counter
	process(clk, rst)
	begin
		if(rst = '1') then
			pit_state_reg <= COUNTING;
			counter_reg <= x"000000FF";
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
			when RELOAD  =>
				if(reload_en = '1') then
					pit_state_next <= COUNTING;
				else
					pit_state_next <= RELOAD;
				end if;
		end case;
	end process;
	
	-- Output Logic
	process(counter_en, reload_en, pit_state_reg, interrupt_en, counter_reg)
	begin
		intr <= '0'; -- Default Value
		case pit_state_reg is
			when COUNTING =>
				if (counter_en = '1') then
					counter_next <= counter_reg - 1;
				else
					counter_next <= counter_reg;
				end if;
			when INTERRUPT =>
				intr <= '1' and interrupt_en;
			when RELOAD  =>
				if(reload_en = '1') then
					counter_next <= delay_reg;
				end if;
		end case;
	end process;
	
end Behavioral;