
module onchipAlarm (
	clk_clk,
	horas_d_export,
	led_export,
	horas_u_export,
	modo_export,
	push_buttons_export,
	reset_reset_n,
	minutos_d_export,
	minutos_u_export,
	segundos_d_export,
	segundos_u_export);	

	input		clk_clk;
	output	[6:0]	horas_d_export;
	output		led_export;
	output	[6:0]	horas_u_export;
	input		modo_export;
	input	[3:0]	push_buttons_export;
	input		reset_reset_n;
	output	[6:0]	minutos_d_export;
	output	[6:0]	minutos_u_export;
	output	[6:0]	segundos_d_export;
	output	[6:0]	segundos_u_export;
endmodule
