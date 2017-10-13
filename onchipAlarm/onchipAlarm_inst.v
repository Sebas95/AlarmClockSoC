	onchipAlarm u0 (
		.clk_clk             (<connected-to-clk_clk>),             //          clk.clk
		.horas_d_export      (<connected-to-horas_d_export>),      //      horas_d.export
		.led_export          (<connected-to-led_export>),          //          led.export
		.horas_u_export      (<connected-to-horas_u_export>),      //      horas_u.export
		.modo_export         (<connected-to-modo_export>),         //         modo.export
		.push_buttons_export (<connected-to-push_buttons_export>), // push_buttons.export
		.reset_reset_n       (<connected-to-reset_reset_n>),       //        reset.reset_n
		.minutos_d_export    (<connected-to-minutos_d_export>),    //    minutos_d.export
		.minutos_u_export    (<connected-to-minutos_u_export>),    //    minutos_u.export
		.segundos_d_export   (<connected-to-segundos_d_export>),   //   segundos_d.export
		.segundos_u_export   (<connected-to-segundos_u_export>)    //   segundos_u.export
	);

