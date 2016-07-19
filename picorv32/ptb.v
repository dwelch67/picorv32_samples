
module ptb
(
	input clk,
    input resetn,
	input         mem_ready        ,
	input [31:0] mem_rdata         ,
	output        trap         ,
	output        mem_valid    ,
	output        mem_instr    ,
	output [31:0] mem_addr     ,
	output [31:0] mem_wdata    ,
	output [ 3:0] mem_wstrb    
);
	//reg        mem_ready         ;
	//reg [31:0] mem_rdata         ;
	reg        pcpi_wr           ;
	reg [31:0] pcpi_rd           ;
	reg        pcpi_wait         ;
	reg        pcpi_ready        ;
	reg [31:0] irq               ;

	//wire        trap              ;
	//wire        mem_valid         ;
	//wire        mem_instr         ;
	//wire [31:0] mem_addr          ;
	//wire [31:0] mem_wdata         ;
	//wire [ 3:0] mem_wstrb         ;
	wire        mem_la_read       ;
	wire        mem_la_write      ;
	wire [31:0] mem_la_addr       ;
	wire [31:0] mem_la_wdata      ;
	wire [ 3:0] mem_la_wstrb      ;
	wire        pcpi_valid        ;
	wire [31:0] pcpi_insn         ;
	wire [31:0] pcpi_rs1          ;
	wire [31:0] pcpi_rs2          ;
	wire [31:0] eoi               ;

picorv32 prv
(
    .clk(clk),
    .resetn(resetn),
    
	.mem_ready       (mem_ready       ),
	.mem_rdata       (mem_rdata       ),
	.pcpi_wr         (pcpi_wr         ),
	.pcpi_rd         (pcpi_rd         ),
	.pcpi_wait       (pcpi_wait       ),
	.pcpi_ready      (pcpi_ready      ),
	.irq             (irq             ),
	.trap            (trap            ),
	.mem_valid       (mem_valid       ),
	.mem_instr       (mem_instr       ),
	.mem_addr        (mem_addr        ),
	.mem_wdata       (mem_wdata       ),
	.mem_wstrb       (mem_wstrb       ),
	.mem_la_read     (mem_la_read     ),
	.mem_la_write    (mem_la_write    ),
	.mem_la_addr     (mem_la_addr     ),
	.mem_la_wdata    (mem_la_wdata    ),
	.mem_la_wstrb    (mem_la_wstrb    ),
	.pcpi_valid      (pcpi_valid      ),
	.pcpi_insn       (pcpi_insn       ),
	.pcpi_rs1        (pcpi_rs1        ),
	.pcpi_rs2        (pcpi_rs2        ),
	.eoi             (eoi             )
);



//assign mem_ready   = 0;
	//assign mem_rdata   = 0;
	assign pcpi_wr     = 0;
	assign pcpi_rd     = 0;
	assign pcpi_wait   = 0;
	assign pcpi_ready  = 0;
	assign irq         = 0;
         

endmodule
