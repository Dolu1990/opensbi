#include <sbi/riscv_io.h>
#include <sbi/sbi_console.h>

static volatile unsigned long *uart_base;

#define UART_DATA 0
#define UART_STATUS 1


static void spinal_uart_putc(char ch)
{
	while(((readl(uart_base + UART_STATUS) >> 16) & 0xFF) == 0);
	writel(ch, uart_base);
}

static int spinal_uart_getc(void)
{
    return (readl(uart_base + UART_STATUS) >> 24) == 0 ? -1 : readl(uart_base + UART_DATA);
}

static struct sbi_console_device spinal_console = {
	.name = "spinal_uart",
	.console_putc = spinal_uart_putc,
	.console_getc = spinal_uart_getc
};

int spinal_uart_init(unsigned long base)
{
	uart_base = (unsigned long *)base;
	sbi_console_set_device(&spinal_console);
	return 0;
}
