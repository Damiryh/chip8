#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>

typedef unsigned char byte;
typedef unsigned short word;

FILE *bin;
byte buffer[0x1000];
int pos;

bool with_labels = false;
bool with_addr = false;
bool with_form = false;
std::set<word> labels;

word rb() {
	return buffer[pos++];
}

word read() {
	byte high = rb();
	byte low = rb();
	return (high << 8) | low;
}

word nnn(word op) {
	if (with_labels) labels.insert(op & 0xfff);
	return op & 0xfff;
}

byte n(word op) { return op & 0x0f; }
byte x(word op) { return (op >> 8) & 0x0f; }
byte y(word op) { return (op >> 4) & 0x0f; }
byte kk(word op) { return op & 0xff; }

void usage() {
	printf("%s\n", "Usage: chip8disasm [options] <ch8 file>");
	exit(1);
}

int main(int argc, char** argv) {
	if (argc < 2) usage();
	
	for (int i = 1; i < argc-1; i++) {
		if (!strcmp("-L", argv[i])) { with_labels = true; } else
		if (!strcmp("-A", argv[i])) { with_addr = true; } else
		if (!strcmp("-F", argv[i])) { with_form = true; }
		else usage();
	}
	
	bin = fopen(argv[argc-1], "rb");
	if (bin == NULL) {
		printf("%s\n", "Cannot open this file.");
		return 1;
	}
	
	int length = fread(buffer, 1, 0x1000, bin);
	printf("Length: %d bytes.\n", length);
	
	fclose(bin);
	
	while (pos < length) {
		if (with_addr) {
			printf("; #%04x\n", pos + 0x200);
		}
		
		word op = read();
		
		if (with_form) {
			printf("    ");
		}
		
		if ((op & 0xffff) == 0x00e0) {
			printf("cls\n");
		} else
		if ((op & 0xffff) == 0x00ee) {
			printf("ret\n");
		} else
		if ((op & 0xf000) == 0x1000) {
			printf("jp [#%04x]\n", nnn(op));
		} else
		if ((op & 0xf000) == 0x2000) {
			printf("call [#%04x]\n", nnn(op));
		} else
		if ((op & 0xf000) == 0x3000) {
			printf("se @%d, #%02x\n", x(op), kk(op));
		} else
		if ((op & 0xf000) == 0x4000) {
			printf("sne @%d, #%02x\n", x(op), kk(op));
		} else
		if ((op & 0xf00f) == 0x5000) {
			printf("se @%d, @%d\n", x(op), y(op));
		} else
		if ((op & 0xf000) == 0x6000) {
			printf("ld @%d, #%02x\n", x(op), kk(op));
		} else
		if ((op & 0xf000) == 0x7000) {
			printf("add @%d, #%02x\n", x(op), kk(op));
		} else
		if ((op & 0xf00f) == 0x8000) {
			printf("ld @%d, @%d\n", x(op), y(op));
		} else
		if ((op & 0xf00f) == 0x8001) {
			printf("or @%d, @%d\n", x(op), y(op));
		} else
		if ((op & 0xf00f) == 0x8002) {
			printf("and @%d, @%d\n", x(op), y(op));
		} else
		if ((op & 0xf00f) == 0x8003) {
			printf("xor @%d, @%d\n", x(op), y(op));
		} else
		if ((op & 0xf00f) == 0x8004) {
			printf("add @%d, @%d\n", x(op), y(op));
		} else
		if ((op & 0xf00f) == 0x8005) {
			printf("sub @%d, @%d\n", x(op), y(op));
		} else
		if ((op & 0xf00f) == 0x8006) {
			printf("shr @%d, @%d\n", x(op), y(op));
		} else 
		if ((op & 0xf00f) == 0x8007) {
			printf("subn @%d, @%d\n");
		} else 
		if ((op & 0xf00f) == 0x800e) {
			printf("shl @%d, @%d\n", x(op), y(op));
		} else 
		if ((op & 0xf00f) == 0x9000) {
			printf("sne @%d, @%d\n", x(op), y(op));
		} else 
		if ((op & 0xf000) == 0xa000) {
			printf("ld I, [#%04x]\n", nnn(op));
		} else 
		if ((op & 0xf000) == 0xb000) {
			printf("jp @0, [#%04x]\n", nnn(op));
		} else 
		if ((op & 0xf000) == 0xc000) {
			printf("rnd @%d, #%02x\n", x(op), kk(op));
		} else 
		if ((op & 0xf000) == 0xd000) {
			printf("drw @%d, @%d, !%d\n", x(op), y(op), n(op));
		} else 
		if ((op & 0xf0ff) == 0xe09e) {
			printf("skp @%d\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xe0a1) {
			printf("sknp @%d\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xf007) {
			printf("ld @%d, DT\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xf00a) {
			printf("ld @%d, K\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xf015) {
			printf("ld DT, @%d\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xf018) {
			printf("ld ST, @%d\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xf01e) {
			printf("add I, @%d\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xf029) {
			printf("ld F, @%d\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xf033) {
			printf("ld B, @%d\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xf055) {
			printf("ld [I], @%d\n", x(op));
		} else 
		if ((op & 0xf0ff) == 0xf065) {
			printf("ld @%d, [I]\n", x(op));
		}
		else {
			printf(".db #%02x, #%02x\n", (op >> 8) & 0xff, op & 0xff);
		}
	}
	
	if (with_labels) {
		printf("; Labels:\n");
		for (auto label: labels) {
			printf("; label_%04x = #%04x\n", label, label);
		}
	}
	
	return 0;
}