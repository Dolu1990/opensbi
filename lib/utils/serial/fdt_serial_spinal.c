#include <sbi/sbi_error.h>
#include <sbi_utils/fdt/fdt_helper.h>
#include <sbi_utils/serial/fdt_serial.h>

extern int spinal_uart_init(unsigned long base);

static int serial_spinal_init(void *fdt, int nodeoff,
			     const struct fdt_match *match)
{
	uint64_t reg_addr, reg_size;
	int rc;

	if (nodeoff < 0 || !fdt)
		return SBI_ENODEV;

	rc = fdt_get_node_addr_size(fdt, nodeoff, 0, &reg_addr, &reg_size);
	if (rc < 0 || !reg_addr || !reg_size)
		return SBI_ENODEV;

	return spinal_uart_init(reg_addr);
}

static const struct fdt_match serial_spinal_match[] = {
	{ .compatible = "spinal,uart" },
	{ },
};

struct fdt_serial fdt_serial_spinal = {
	.match_table = serial_spinal_match,
	.init = serial_spinal_init
};
