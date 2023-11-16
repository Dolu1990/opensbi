/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021 Gabriel Somlo
 *
 * Authors:
 *   Gabriel Somlo <gsomlo@gmail.com>
 */

#include <sbi/riscv_io.h>
#include <sbi/sbi_console.h>
#include <sbi_utils/serial/litex-uart.h>

/* clang-format off */

#define UART_REG_RXTX		0
#define UART_REG_TXFULL		1
#define UART_REG_RXEMPTY	2
#define UART_REG_EV_STATUS	3
#define UART_REG_EV_PENDING	4
#define UART_REG_EV_ENABLE	5

#define UART_EV_TX	0x1
#define UART_EV_RX	0x2


/* clang-format on */

static volatile u32 *uart_base;

static u32 get_reg(u32 reg)
{
	return readl(uart_base + reg);
}

static void set_reg(u32 reg, u32 val)
{
	writel(val, uart_base + reg);
}

static void litex_uart_putc(char ch)
{
	while (get_reg(UART_REG_TXFULL));
	set_reg(UART_REG_RXTX, ch);
	set_reg(UART_REG_EV_PENDING, UART_EV_TX);
}

static int litex_uart_getc(void)
{
	if (get_reg(UART_REG_RXEMPTY))
		return -1;

	u32 rx = get_reg(UART_REG_RXTX);
	set_reg(UART_REG_EV_PENDING, UART_EV_RX);
	return rx;
}

static struct sbi_console_device litex_console = {
	.name = "litex_uart",
	.console_putc = litex_uart_putc,
	.console_getc = litex_uart_getc
};

int litex_uart_init(unsigned long base)
{
	uart_base = (volatile u32 *)base;
	set_reg(UART_REG_EV_ENABLE, 0); /* UART in polling mode: disable IRQ */
	sbi_console_set_device(&litex_console);
	return 0;
}
