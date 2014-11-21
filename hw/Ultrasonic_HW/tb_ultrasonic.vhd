--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   13:26:35 11/15/2014
-- Design Name:   
-- Module Name:   C:/RH/Lab/hw/Ultrasonic_HW/tb_ultrasonic.vhd
-- Project Name:  Ultrasonic_HW
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: ultrasonic_hw
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY tb_ultrasonic IS
END tb_ultrasonic;
 
ARCHITECTURE behavior OF tb_ultrasonic IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT ultrasonic_hw
    PORT(
         clk : IN  std_logic;
         echo : IN  std_logic;
         trigger : OUT  std_logic;
         distance_reg : OUT  std_logic_vector(31 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal echo : std_logic := '0';

 	--Outputs
   signal trigger : std_logic;
   signal distance_reg : std_logic_vector(31 downto 0);

   -- Clock period definitions
   constant clk_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: ultrasonic_hw PORT MAP (
          clk => clk,
          echo => echo,
          trigger => trigger,
          distance_reg => distance_reg
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- insert stimulus here 
		wait for 15000000 ns;	-- wait for trigger to send
		echo <= '1';
		wait for 10 ms;
		echo <= '0';
		
		wait for 15000000 ns;
		echo <= '1';
		wait for 2ms;
		echo <= '0';

		wait for 15000000 ns;
		echo <= '1';
		wait for 300000 ns;
		echo <= '0';

		wait for 15000000 ns;
		echo <= '1';
		wait for 20ms;
		echo <= '0';

      wait;
   end process;

END;
