#include <X11/Intrinsic.h>
#include <X11/Xos.h>
#include <Xm/Xm.h>
#include <X11/StringDefs.h>
#include <stdio.h>

#define DEF_BLACK       BlackPixel(dsp, DefaultScreen(dsp))
#define DEF_WHITE       WhitePixel(dsp, DefaultScreen(dsp))
#define	MAX_LINE	81


extern unsigned char *ReadGifBitmap();


extern Display *dsp;


char nibMask[8] = {
	1, 2, 4, 8, 16, 32, 64, 128
};


unsigned char *ReadXpmPixmap(fp, datafile, w, h, colrs, Colors, CharsPP)
FILE *fp;
char *datafile;
int *w, *h;
XColor *colrs;
int Colors, CharsPP;
{
	unsigned char *pixels;
	char **Color_Vals;
	XColor tmpcolr;
	int i, j, k;
	int value, found;
	char line[BUFSIZ], name_and_type[MAX_LINE];
	unsigned char *dataP;
	unsigned char *bitp;
	int tchar;
	char *t;
	char *t2;

	if (Colors == 0)
	{
		fprintf(stderr, "Can't find Colors.\n");
		return((unsigned char *)NULL);
	}
	if (*w == 0)
	{
		/* fprintf(stderr, "Can't read image.\n"); */
		return((unsigned char *)NULL);
	}
	if (*h == 0)
	{
		/* fprintf(stderr, "Can't read image.\n"); */
		return((unsigned char *)NULL);
	}

	Color_Vals = (char **)malloc(sizeof(char *) * Colors);
	for (i=0; i<Colors; i++)
	{
		tchar = getc(fp);
		while ((tchar != '"')&&(tchar != EOF))
		{
			tchar = getc(fp);
		}
		Color_Vals[i] = (char *)malloc(sizeof(char) * (CharsPP + 1));
		j = 0;
		tchar = getc(fp);
		while ((tchar != '"')&&(tchar != EOF)&&(j < CharsPP))
		{
			Color_Vals[i][j] = (char)tchar;
			tchar = getc(fp);
			j++;
		}
		Color_Vals[i][j] = '\0';
		if (tchar != '"')
		{
			tchar = getc(fp);
			while ((tchar != '"')&&(tchar != EOF))
			{
				tchar = getc(fp);
			}
		}
		tchar = getc(fp);
		while ((tchar != '"')&&(tchar != EOF))
		{
			tchar = getc(fp);
		}
		j = 0;
		tchar = getc(fp);
		while ((tchar != '"')&&(tchar != EOF))
		{
			line[j] = (char)tchar;
			tchar = getc(fp);
			j++;
		}
		line[j] = '\0';
		XParseColor(dsp, DefaultColormap(dsp, DefaultScreen(dsp)),
			line, &tmpcolr);
		colrs[i].red = tmpcolr.red;
		colrs[i].green = tmpcolr.green;
		colrs[i].blue = tmpcolr.blue;
		colrs[i].pixel = i;
		colrs[i].flags = DoRed|DoGreen|DoBlue;
	}
	for (i=Colors; i<256; i++)
	{
		colrs[i].red = 0;
		colrs[i].green = 0;
		colrs[i].blue = 0;
		colrs[i].pixel = i;
		colrs[i].flags = DoRed|DoGreen|DoBlue;
	}
	tchar = getc(fp);
	while ((tchar != ';')&&(tchar != EOF))
	{
		tchar = getc(fp);
	}

	for ( ; ; )
	{
		if (!(fgets(line, MAX_LINE, fp)))
		{
			fprintf(stderr, "Can't find Pixels\n");
			return((unsigned char *)NULL);
		}
		else if (sscanf(line,"static char * %s = {",name_and_type) == 1)
		{
			if ((t = rindex(name_and_type, '_')) == NULL)
			{
				t = name_and_type;
			}
			else
			{
				t++;
			}
			if ((t2 = index(name_and_type, '[')) != NULL)
			{
				*t2 = '\0';
			}
			if (!strcmp("pixels", t))
			{
				break;
			}
		}
	}
	pixels = (unsigned char *)malloc((*w) * (*h));
	if (pixels == NULL)
	{
		fprintf(stderr, "Not enough memory for data.\n");
		return((unsigned char *)NULL);
	}

	line[0] = '\0';
	t = line;
	dataP = pixels;
	tchar = getc(fp);
	while ((tchar != '"')&&(tchar != EOF))
	{
		tchar = getc(fp);
	}
	tchar = getc(fp);
	for (j=0; j<(*h); j++)
	{
		for (i=0; i<(*w); i++)
		{
			k = 0;
			while ((tchar != '"')&&(tchar != EOF)&&(k < CharsPP))
			{
				line[k] = (char)tchar;
				tchar = getc(fp);
				k++;
			}
			if ((k == 0)&&(tchar == '"'))
			{
				tchar = getc(fp);
				while ((tchar != '"')&&(tchar != EOF))
				{
					tchar = getc(fp);
				}
				k = 0;
				tchar = getc(fp);
				while ((tchar != '"')&&(tchar != EOF)&&
					(k < CharsPP))
				{
					line[k] = (char)tchar;
					tchar = getc(fp);
					k++;
				}
			}
			line[k] = '\0';
			found = 0;
			for (k=0; k<Colors; k++)
			{
				if (strncmp(Color_Vals[k], line, CharsPP) == 0)
				{
					*dataP++ = (unsigned char)k;
					found = 1;
					break;
				}
			}
			if (found == 0)
			{
				fprintf(stderr, "Invalid Pixel (%2s) in file %s\n", line, datafile);
				*dataP++ = (unsigned char)0;
			}
		}
	}

	bitp = pixels;
	for (i=0; i<((*w) * (*h)); i++)
	{
		if ((int)*bitp > (256 - 1))
			*bitp = (unsigned char)0;
		bitp++;
	}

	for (i=0; i<Colors; i++)
	{
		free((char *)Color_Vals[i]);
	}
	free((char *)Color_Vals);
	return(pixels);
}


unsigned char *ReadXbmBitmap(fp, datafile, w, h, colrs)
FILE *fp;
char *datafile;
int *w, *h;
XColor *colrs;
{
	char line[MAX_LINE], name_and_type[MAX_LINE];
	char *t;
	char *t2;
	unsigned char *ptr, *dataP;
	int bytes_per_line, version10p, raster_length, padding;
	int i, bytes, temp, value;
	int Ncolors, charspp, xpmformat;
        static unsigned long fg_pixel, bg_pixel;
        static int done_fetch_colors = 0;
        extern XColor fg_color, bg_color;
        extern Widget view;
        extern int Vclass;
	int blackbit;
	int whitebit;

        if (!done_fetch_colors)
          {
            /* First, go fetch the pixels. */
            XtVaGetValues (view, XtNforeground, &fg_pixel,
                         XtNbackground, &bg_pixel, NULL);
            
            /* Now, load up fg_color and bg_color. */
            fg_color.pixel = fg_pixel;
            bg_color.pixel = bg_pixel;
            
            /* Now query for the full color info. */
            XQueryColor 
              (XtDisplay (view), 
               DefaultColormap (XtDisplay (view),
                                DefaultScreen (XtDisplay (view))),
               &fg_color);
            XQueryColor 
              (XtDisplay (view), 
               DefaultColormap (XtDisplay (view),
                                DefaultScreen (XtDisplay (view))),
               &bg_color);

            done_fetch_colors = 1;

	    /*
	     * For a TrueColor visual, we can't use the pixel value as
	     * the color index because it is > 255.  Arbitrarily assign
	     * 0 to foreground, and 1 to background.
	     */
	    if (Vclass == TrueColor)
	      {
		fg_color.pixel = 0;
		bg_color.pixel = 1;
	      }

          }

	blackbit = fg_color.pixel;
	whitebit = bg_color.pixel;

	/*
	 * Error out here on visuals we can't handle so we won't core dump
	 * later.
	 */
	if (((blackbit > 255)||(whitebit > 255))&&(Vclass != TrueColor))
	  {
		fprintf(stderr, "Error:  cannot deal with default colormap that is deeper than 8, and not TrueColor\n");
                fprintf(stderr, "        If you actually have such a system, please notify mosaic-x@ncsa.uiuc.edu.\n");
                fprintf(stderr, "        We thank you for your support.\n");
		exit(1);
	  }

        colrs[blackbit].red = fg_color.red;
        colrs[blackbit].green = fg_color.green;
        colrs[blackbit].blue = fg_color.blue;
        colrs[blackbit].pixel = fg_color.pixel;
	colrs[blackbit].flags = DoRed|DoGreen|DoBlue;

        colrs[whitebit].red = bg_color.red;
        colrs[whitebit].green = bg_color.green;
        colrs[whitebit].blue = bg_color.blue;
        colrs[whitebit].pixel = bg_color.pixel;
	colrs[whitebit].flags = DoRed|DoGreen|DoBlue;

	*w = 0;
	*h = 0;
	Ncolors = 0;
	charspp = 0;
	xpmformat = 0;
	for ( ; ; )
	{
		if (!(fgets(line, MAX_LINE, fp)))
			break;
		if (strlen(line) == (MAX_LINE - 1))
		{
#ifdef DEBUG
			fprintf(stderr, "Line too long.\n");
#endif
			return((unsigned char *)NULL);
		}
		if (sscanf(line, "#define %s %d", name_and_type, &value) == 2)
		{
			if (!(t = rindex(name_and_type, '_')))
				t = name_and_type;
			else
				t++;
			if (!strcmp("width", t))
				*w= value;
			if (!strcmp("height", t))
				*h= value;
			if (!strcmp("ncolors", t))
				Ncolors = value;
			if (!strcmp("pixel", t))
				charspp = value;
			continue;
		}
		if (sscanf(line, "static short %s = {", name_and_type) == 1)
		{
			version10p = 1;
			break;
		}
		else if (sscanf(line,"static char * %s = {",name_and_type) == 1)
		{
			xpmformat = 1;
			if (!(t = rindex(name_and_type, '_')))
				t = name_and_type;
			else
				t++;
			if ((t2 = index(name_and_type, '[')) != NULL)
				*t2 = '\0';
			if (!strcmp("mono", t))
				continue;
			else
				break;
		}
		else if (sscanf(line, "static char %s = {", name_and_type) == 1)
		{
			version10p = 0;
			break;
		}
		else
			continue;
	}
	if (xpmformat)
	{
		dataP = ReadXpmPixmap(fp, datafile, w, h, colrs, Ncolors, charspp);
		return(dataP);
	}
	if (*w == 0)
	{
		/* fprintf(stderr, "Can't read image.\n"); */
		return((unsigned char *)NULL);
	}
	if (*h == 0)
	{
		/* fprintf(stderr, "Can't read image.\n"); */
		return((unsigned char *)NULL);
	}
	padding = 0;
	if (((*w % 16) >= 1)&&((*w % 16) <= 8)&&version10p)
	{
		padding = 1;
	}
	bytes_per_line = ((*w + 7) / 8) + padding;
	raster_length =  bytes_per_line * *h;
	dataP = (unsigned char *)malloc((*w) * (*h));
	if (dataP == NULL)
	{
		fprintf(stderr, "Not enough memory.\n");
		return((unsigned char *)NULL);
	}
	ptr = dataP;
	if (version10p)
	{
		int cnt = 0;
		int lim = (bytes_per_line - padding) * 8;
		for (bytes = 0; bytes < raster_length; bytes += 2)
		{
			if (fscanf(fp, " 0x%x%*[,}]%*[ \n]", &value) != 1)
			{
				fprintf(stderr, "Error scanning bits item.\n");
				return((unsigned char *)NULL);
			}
			temp = value;
			value = temp & 0xff;
			for (i = 0; i < 8; i++)
			{
				if (cnt < (*w))
				{
					if (value & nibMask[i])
						*ptr++ = blackbit;
					else
						*ptr++ = whitebit;
				}
				if (++cnt >= lim)
					cnt = 0;
			}
			if ((!padding)||((bytes+2) % bytes_per_line))
			{
				value = temp >> 8;
				for (i = 0; i < 8; i++)
				{
					if (cnt < (*w))
					{
						if (value & nibMask[i])
							*ptr++ = blackbit;
						else
							*ptr++ = whitebit;
					}
					if (++cnt >= lim)
						cnt = 0;
				}
			}
		}
	}
	else
	{
		int cnt = 0;
		int lim = bytes_per_line * 8;
		for (bytes = 0; bytes < raster_length; bytes++)
		{
			if (fscanf(fp, " 0x%x%*[,}]%*[ \n]", &value) != 1)
			{
				fprintf(stderr, "Error scanning bits item.\n");
				return((unsigned char *)NULL);
			}
			for (i = 0; i < 8; i++)
			{
				if (cnt < (*w))
				{
					if (value & nibMask[i])
						*ptr++ = blackbit;
					else
						*ptr++ = whitebit;
				}
				if (++cnt >= lim)
					cnt = 0;
			}
		}
	}
	return(dataP);
}


unsigned char *ReadBitmap(datafile, w, h, colrs)
char *datafile;
int *w, *h;
XColor *colrs;
{
	unsigned char *bit_data;
	FILE *fp;

        /* Obviously this isn't going to work. */
        fp = fopen(datafile, "r");

	if (fp != NULL)
	{
		bit_data = ReadGifBitmap(fp, datafile, w, h, colrs);
		if (bit_data != NULL)
		{
			if (fp != stdin) fclose(fp);
			return(bit_data);
		}
		rewind(fp);

		bit_data = ReadXbmBitmap(fp, datafile, w, h, colrs);
		if (bit_data != NULL)
		{
			if (fp != stdin) fclose(fp);
			return(bit_data);
		}
	}
	if (fp != stdin) fclose(fp);
	return((unsigned char *)NULL);
}

