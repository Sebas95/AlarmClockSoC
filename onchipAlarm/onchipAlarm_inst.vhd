	component onchipAlarm is
		port (
			clk_clk             : in  std_logic                    := 'X';             -- clk
			horas_d_export      : out std_logic_vector(6 downto 0);                    -- export
			led_export          : out std_logic;                                       -- export
			horas_u_export      : out std_logic_vector(6 downto 0);                    -- export
			modo_export         : in  std_logic                    := 'X';             -- export
			push_buttons_export : in  std_logic_vector(3 downto 0) := (others => 'X'); -- export
			reset_reset_n       : in  std_logic                    := 'X';             -- reset_n
			minutos_d_export    : out std_logic_vector(6 downto 0);                    -- export
			minutos_u_export    : out std_logic_vector(6 downto 0);                    -- export
			segundos_d_export   : out std_logic_vector(6 downto 0);                    -- export
			segundos_u_export   : out std_logic_vector(6 downto 0)                     -- export
		);
	end component onchipAlarm;

	u0 : component onchipAlarm
		port map (
			clk_clk             => CONNECTED_TO_clk_clk,             --          clk.clk
			horas_d_export      => CONNECTED_TO_horas_d_export,      --      horas_d.export
			led_export          => CONNECTED_TO_led_export,          --          led.export
			horas_u_export      => CONNECTED_TO_horas_u_export,      --      horas_u.export
			modo_export         => CONNECTED_TO_modo_export,         --         modo.export
			push_buttons_export => CONNECTED_TO_push_buttons_export, -- push_buttons.export
			reset_reset_n       => CONNECTED_TO_reset_reset_n,       --        reset.reset_n
			minutos_d_export    => CONNECTED_TO_minutos_d_export,    --    minutos_d.export
			minutos_u_export    => CONNECTED_TO_minutos_u_export,    --    minutos_u.export
			segundos_d_export   => CONNECTED_TO_segundos_d_export,   --   segundos_d.export
			segundos_u_export   => CONNECTED_TO_segundos_u_export    --   segundos_u.export
		);

