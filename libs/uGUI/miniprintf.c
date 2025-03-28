/* Minimal printf() facility for MCUs
 * Warren W. Gay VE3WWG,  Sun Feb 12 2017
 *
 * This work is placed in the public domain. No warranty, or guarantee
 * is expressed or implied. When uou use this source code, you do so
 * with full responsibility and at your own risk.
 */

#include "miniprintf.h"

/*
 * Internal structure for I/O
 */
struct s_mini_args {
	void	(*putc)(char,void *);	// The putc() function to invoke
	void 	*argp;			// Associated data struct
};

typedef struct s_mini_args miniarg_t;	// Abbreviated ref to s_mini_args

/*
 * Internal: Write string msg until null byte, to the I/O
 *           routine described by s_mini_args.
 */
static void
mini_write(miniarg_t *mini,const char *msg) {
	char ch;

	while ( (ch = *msg++) != 0 )
		mini->putc(ch,mini->argp);
}

/*
 * Internal: Pad % field to width, give text buffer.
 */
static void
mini_pad(miniarg_t *mini,char pad,int width,const char *text) {
	int slen;

	if ( width > 0 ) {
		slen = strlen(text);

		for ( width -= slen; width > 0; --width )
			mini->putc(pad,mini->argp);
	}
}

/*
 * Internal: mini_printf() engine.
 */
static void
internal_vprintf(miniarg_t *mini,const char *format,va_list arg) {
	char ch, pad, sgn;	/* Current char, pad char and sign char */
	int vint, width;	/* Integer value to print and field width */
	unsigned uint;		/* Unsigned value to print */
	const char *sptr;	/* String to print */
	char buf[32], *bptr;	/* Formatting buffer for int/uint */

	while ( (ch = *format++) != 0 ) {
		if ( ch != '%' ) {
			/* Non formatting field: copy as is */
			mini->putc(ch,mini->argp);
			continue;
		}

		/*
		 * Process a format item:
		 */
		pad = ' ';	/* Default pad char is space */
		sgn = 0;	/* Assume no format sign char */
		ch = *format++;	/* Grab next format char */

		if ( ch == '+' || ch == '-' ) {
			sgn = ch;	/* Make note of format sign */
			ch = *format++;	/* Next format char */
		}

		if ( ch == '0' ) {
			pad = ch;	/* Pad with zeros */
			ch = *format++;
		}

		/*
		 * Extract width when present:
		 */
		for ( width = 0; ch && ch >= '0' && ch <= '9'; ch = *format++ )
			width = width * 10 + (ch & 0x0F);

		if ( !ch )
			break;		/* Exit loop if we hit end of format string (in error) */

		/*
		 * Format according to type: d, x, or s
		 */
		switch ( ch ) {
		case 'c':
			vint = va_arg(arg,int);
			mini->putc((char)vint,mini->argp);
			break;
		case 'd':		/* Decimal format */
			vint = va_arg(arg,int);
			if ( vint < 0 ) {
				mini->putc('-',mini->argp);
				vint = -vint;
			} else if ( sgn == '+' )
				mini->putc(sgn,mini->argp);
			bptr = buf + sizeof buf;
			*--bptr = 0;
			do	{
				*--bptr = vint % 10 + '0';
				vint /= 10;
			} while ( vint != 0 );
			mini_pad(mini,pad,width,bptr);
			mini_write(mini,bptr);
			break;

		case 'x':		/* Hexadecimal format */
			uint = va_arg(arg,unsigned);
			bptr = buf + sizeof buf;
			*--bptr = 0;
			do	{
				ch = uint & 0x0F;
				*--bptr = ch + (ch <= 9 ? '0' : 'A'-10);
				uint >>= 4;
			} while ( uint != 0 );
			mini_pad(mini,pad,width,bptr);
			mini_write(mini,bptr);
			break;

		case 's':		/* String format */
			sptr = va_arg(arg,const char *);
			if ( sgn != '-' )
				mini_pad(mini,pad,width,sptr);
			mini_write(mini,sptr);
			if ( sgn == '-' )
				mini_pad(mini,pad,width,sptr);
			break;

		case '%':		/* "%%" outputs as "%" */
			mini->putc(ch,mini->argp);
			break;

		default:		/* Unsupported stuff here */
			mini->putc('%',mini->argp);
			mini->putc('?',mini->argp);
			mini->putc(ch,mini->argp);
		}
	}
}

/*
 * s_internal trackes the count of bytes output:
 */
struct s_internal {
	void (*putc)(char);	/* User's putc() routine to be used */
	unsigned count;		/* Bytes output */
	unsigned cooked : 1;	/* When true, '\n' also emits '\r' */
};

static void
mini_putc(char ch,void *argp) {
	struct s_internal *internp = (struct s_internal *)argp;

	internp->putc(ch);	/* Perform I/O */
	++internp->count;	/* Count bytes out */

	if ( ch == '\n' && internp->cooked != 0 ) {
		/* In cooked mode, issue CR after LF */
		internp->putc('\r');
		++internp->count;	/* Count CR */
	}
}

/*
 * Internal: Perform cooked/uncooked printf()
 */
static int
mini_vprintf0(void (*putc)(char),int cooked,const char *format,va_list args) {
	miniarg_t mini;
	struct s_internal intern;

	intern.putc = putc;		/* User's putc() routine to be used */
	intern.count = 0u;		/* Byte counter */
	intern.cooked = !!cooked; 	/* True if LF to add CR */

	mini.putc = mini_putc;		/* Internal interlude routine */
	mini.argp = (void *)&intern; 	/* Ptr to internal struct */

	internal_vprintf(&mini,format,args);
	return intern.count;		/* Return byte count */
}

/*
 * External: Perform cooked mode printf()
 */
int
mini_vprintf_cooked(void (*putc)(char),const char *format,va_list args) {
	return mini_vprintf0(putc,1,format,args);
}

/*
 * External: Perform uncooked (as is) printf()
 */
int
mini_vprintf_uncooked(void (*putc)(char),const char *format,va_list args) {
	return mini_vprintf0(putc,0,format,args);
}

/*********************************************************************
 * Sprintf
 *********************************************************************/

struct s_mini_sprintf {
	char	*buf;			/* Ptr to output buffer */
	unsigned maxbuf;		/* Max bytes for buffer */
	char	*ptr;			/* Ptr to next byte */
};

static void
mini_sputc(char ch,void *argp) {
	struct s_mini_sprintf *ctl = (struct s_mini_sprintf *)argp;

	if ( (unsigned)(ctl->ptr - ctl->buf) >= ctl->maxbuf )
		return;
	*ctl->ptr++ = ch;
}

/*
 * External: sprintf() to buffer (not cooked)
 */
int
mini_snprintf(char *buf,unsigned maxbuf,const char *format,...) {
	miniarg_t mini;			/* printf struct */
	struct s_mini_sprintf ctl;	/* sprintf control */
	va_list args;			/* format arguments */
	unsigned count;			/* Return count */

	mini.putc = mini_sputc;		/* Internal routine */
	mini.argp = (void *)&ctl;	/* Using ctl to guide it */

	ctl.ptr = ctl.buf = buf;	/* Destination for data */
	ctl.maxbuf = maxbuf;		/* Max size in bytes */

	va_start(args,format);
	internal_vprintf(&mini,format,args);
	va_end(args);

	count = (unsigned)(ctl.ptr - ctl.buf); /* Calculate count */
	mini_sputc(0,&ctl);		/* Null terminate output if possible */
	return count;			/* Return formatted count */
}

/* End miniprintf.c */

#if (0) /* feat I.Kostromin */
#include <stdint.h>

static void dbgPrintInt(uint32_t val, uint8_t radix, uint8_t padd, uint8_t paddmode) {
    uint32_t tmp, len;
    char chr;
    /* calculate length of the value in radixes */
    tmp = val;
    len = 0;
    while (tmp > 0) {
        tmp /= radix;
        len++;
    }
    if (val == 0) {
    	len = 1;
    }
    /* add radixes padding */
    if (len < padd) {
        padd -= len;
        while (padd > 0) {
            debugUartChar(paddmode);
            padd--;
        }
    }
    tmp = 1;
    while (len > 1) {
        tmp *= radix;
        len--;
    }
    while (tmp > 0) {
        len = val / tmp;
        val -= len * tmp;
        chr = len + '0';
        if (chr > '9') {
        	chr += 7;
        }
        debugUartChar(chr);
        tmp /= radix;
    }
}

static void dbgPrintStr(const char* str) {
	while (str[0] != 0) {
		debugUartChar(str[0]);
		str++;
	}
}

static void dbgHexStr(const uint8_t* addr, uint32_t length) {
    while (length > 0) {
        dbgPrintInt(addr[0], 16, 2, '0');
        addr++;
        length--;
    }
}

static void dbgHexWordStr(const uint32_t* addr, uint32_t wlen) {
    while (wlen > 0) {
        dbgPrintInt(addr[0], 16, 8, '0');
        debugUartChar(' ');
        addr++;
        wlen--;
    }
}

/**
 * supported formats: %i,%d,%I,%D - integer, %s,%S - string, %x,%X - hex, all other not supported,
 * modifiers: [cnt] - length of the resulting number, [0cnt] - same, but use '0' istead of ' ', cnt could be only 1 digit long
 * NOTE: additional: %h,%H - hex arrays, 2 additional arguments - pointer to array and number of bytes within
 * NOTE: additional: %w,%W - word hex arrays, 2 additional arguments - pointer to array and number of words within
 */
void dbgPrint(const char* format, ...) {
    uint8_t radix, padd, paddmode;
    va_list factor;
    va_start(factor, format);
    
    while(format[0] != 0)
    {
        /* format specifier %[flags][width][.precision][length] */
    	if (format[0] == '\n') {
    		debugUartChar('\r');
    	}
        if (format[0] != '%') {
            debugUartChar(format[0]);
            format++;
            continue;
        }
        format++;
        paddmode = ' ';
        if (format[0] == '0') { /* use '0' to pad */
            paddmode = '0';
            format++;
        }
        padd = 0;
        if (format[0] > '0' && format[0] <= '9') {
            padd = format[0] - '0';
            format++;
        }
        radix = 0;
        switch(format[0]) {
            case 'x':
            case 'X':
                radix = 16;
                break;
            case 'i':
            case 'I':
            case 'd':
            case 'D':
                radix = 10;
                break;
            case 's':
            case 'S':
                radix = 1;
                break;
            case 'h':
            case 'H':
                radix = 2;
                break;
            case 'w':
            case 'W':
                radix = 3;
                break;
            default: break;
        }
        format++;
        if (radix > 3) {
            uint32_t digit = va_arg(factor, uint32_t);
        	dbgPrintInt(digit, radix, padd, paddmode);
        } else if (radix == 1) {
        	const char* str = va_arg(factor, const char*);
        	dbgPrintStr(str);
        } else if (radix == 2) {
            const uint8_t* addr = va_arg(factor, const uint8_t*);
            uint32_t length = va_arg(factor, uint32_t);
            dbgHexStr(addr, length);
        } else if (radix == 3) {
            const uint32_t* waddr = va_arg(factor, const uint32_t*);
            uint32_t length = va_arg(factor, uint32_t);
            dbgHexWordStr(waddr, length);
        }
    }
    va_end(factor);
}
#endif /* feat I.Kostromin */