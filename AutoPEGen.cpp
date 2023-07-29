#include<stdio.h>

#define NONZERO_WEIGHT_NUM 16
#define PE_ROW_NUM 8
#define PE_COL_NUM 4
#define S_NUM 14
#define ADDER_ROUND_FIRST 8
#define ADDER_ROUND_TWO 4
#define ADDER_ROUND_THREE 2
#define ADDER_RIN 4
int main() {
	FILE* fp;
	errno_t	err=fopen_s(&fp, "1.txt", "w");

	if (err == 0) {
		fprintf(fp, "wire    [IndexWidth-1 : 0]                  pattern_reg;\n");
		fprintf(fp, "reg    [IndexWidth-1 : 0]                  pattern_reg0;\n");
		fprintf(fp, "reg    [IndexWidth-1 : 0]                  pattern_reg1;\n");
		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "wire    [DataWidth-1 : 0]                   value%d_reg;\n", i);
		}
		fprintf(fp, "\n\n\n");

		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "reg    [DataWidth-1 : 0]                   value%d_reg0;\n", i);
		}
		fprintf(fp, "\n\n\n");

		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "reg    [DataWidth-1 : 0]                   value%d_reg1;\n", i);
		}
		fprintf(fp, "\n\n\n");

		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "reg     [DataWidth-1 : 0]                   A_%d_reg;\n", i);
			fprintf(fp, "reg     [DataWidth-1 : 0]                   B_%d_reg;\n", i);
		}

		for (int i = 0; i < PE_ROW_NUM; i++) {
			fprintf(fp, "reg     [DataWidth-1 : 0]                   b%d_out_reg ;\n", i);
		}
		fprintf(fp, "\n\n\n");

		for (int i = 0; i < PE_COL_NUM; i++) {
			fprintf(fp, "reg     [DataWidth-1 : 0]                   r%d_in ;\n", i);
		}
		fprintf(fp, "\n\n\n");

		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "wire    [ResultWidth-1 : 0]                   P_%d    ;\n", i);
		}
		fprintf(fp, "\n\n\n");

		for (int i = 0; i < S_NUM; i++) {
			fprintf(fp, "wire    [ResultWidth-1 : 0]                   S%d    ;\n", i);
		}
		fprintf(fp, "\n\n\n");

		for (int i = 0; i < PE_ROW_NUM; i++) {
			fprintf(fp, "assign b_out[DataWidth*(%d+1)-1:DataWidth*%d] = b%d_out_reg;\n", i, i, i);
		}
		fprintf(fp, "\n\n\n");

		fprintf(fp, "assign pattern_reg = selector? pattern_reg1 : pattern_reg0  ;\n");

		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "assign value%d_reg = selector ? value%d_reg1 : value%d_reg0;\n", i, i, i);
		}
		fprintf(fp, "\n\n\n");

		fprintf(fp, "always @(posedge clk or negedge rst_n) begin\n");
		fprintf(fp, "    if(!rst_n) begin\n");
		
		for (int i = 0; i < PE_ROW_NUM; i++) {
			fprintf(fp,"        b%d_out_reg          <= 0                        ;\n", i);
		}
		fprintf(fp, "\n");
		fprintf(fp, "        pattern_reg0        <= 0                        ;\n");
		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "        value%d_reg0         <= 0                        ;\n", i);
		}
		fprintf(fp, "\n");
		fprintf(fp, "        pattern_reg1        <= 0                        ;\n");
		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "        value%d_reg1         <= 0                        ;\n", i);
		}
		fprintf(fp, "end\n");
		fprintf(fp, "    else begin\n			if (en)  begin\n");
		for (int i = 0; i < PE_ROW_NUM; i++) {
			fprintf(fp, "            b%d_out_reg          <= b[DataWidth*(%d+1)-1 :DataWidth*%d]                       ;\n", i, i, i);
		}
		fprintf(fp, "            if (selector) begin\n                if (w_en) begin\n                    pattern_reg0    <= pattern  ;\n");
		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "                    value%d_reg0     <= value[DataWidth*(%d+1)-1 :DataWidth*%d]                ;\n", i, i, i);
		}
		fprintf(fp, "                end\n			end\n	else begin\n			if (w_en) begin\n				pattern_reg1 <= pattern;\n");
		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "                    value%d_reg1     <= value[DataWidth*(%d+1)-1 : DataWidth*%d]     ;\n", i, i, i);
		}
		fprintf(fp, "\n\n\n");
		fprintf(fp, "                end\n			end\n			case(pattern_reg)\n  			16'd1: begin			\n");
		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "                A_%d_reg          <= value%d_reg;\n",i,i);
		}
		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "        B_%d_reg          <= b[%d]                    ;\n", i, i % PE_ROW_NUM);
		}
		fprintf(fp,"    end\n    2: begin\n    end\n    endcase\nend\nend\nend\n");
		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "mult_gen_0 mult_%d(\n    .CLK            (clk)                           ,\n    .A              (A_%d_reg)                       ,\n    .B              (B_%d_reg)                       ,\n    .P              (P_%d)\n);\n ",i,i,i,i);
		}
		
		fprintf(fp, "\n");
		for (int i = 0; i < ADDER_ROUND_FIRST; i++) {
			fprintf(fp, "c_addsub_0 adder%d(\n				.A(P_%d),\n				.B(P_%d),\n				.CLK(clk),\n				.CE(1),\n				.S(S%d)\n			);\n\n ", i, 2 * i, 2 * i + 1,i);
		}
		fprintf(fp, "\n");
		for (int i = 0; i < ADDER_ROUND_TWO; i++) {
			fprintf(fp, "c_addsub_0 adder_two%d(\n				.A(S%d),\n				.B(S%d),\n				.CLK(clk),\n				.CE(1),\n				.S(S%d)\n			);\n\n ", i, 2 * i, 2 * i + 1, i + ADDER_ROUND_FIRST);
		}
		for (int i = 0; i < ADDER_ROUND_THREE; i++) {
			fprintf(fp, "c_addsub_0 adder_three%d(\n				.A(S%d),\n				.B(S%d),\n				.CLK(clk),\n				.CE(1),\n				.S(S%d)\n			);\n\n ", i,2*i+ADDER_ROUND_FIRST, 2 * i+1+ADDER_ROUND_FIRST, i + ADDER_ROUND_FIRST+ADDER_ROUND_TWO);
		}
		for (int i = 0; i < ADDER_RIN; i++) {
			fprintf(fp, "c_addsub_0 adder_out%d(\n				.A(S%d),\n				.B(r_in[ResultWidth*(%d+1)-1:ResultWidth*%d]),\n				.CLK(clk),\n				.CE(1),\n				.S(r_out[ResultWidth*(%d+1)-1:ResultWidth*%d])\n			);\n\n ", i, i+ADDER_ROUND_FIRST+ADDER_ROUND_TWO,i,i,i,i);
		}
		fprintf(fp, "assign pattern_below    =   (selector)?     pattern_reg0    :pattern_reg1       ;\n");
		for (int i = 0; i < NONZERO_WEIGHT_NUM; i++) {
			fprintf(fp, "assign value_below[(%d+1)*DataWidth-1:(%d)*DataWidth]   =   (selector)?     value%d_reg0     :value%d_reg1        ;\n", i, i, i, i);
		}
		fprintf(fp, "endmodule\n");
	}
	if (fp) {
		fclose(fp);
	}
	return 0;
}