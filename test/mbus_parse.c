//------------------------------------------------------------------------------
// Copyright (C) 2010, Raditex AB
// All rights reserved.
//
// rSCADA 
// http://www.rSCADA.se
// info@rscada.se
//
//------------------------------------------------------------------------------

#include <sys/types.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mbus/mbus-protocol.h>

int
main(int argc, char *argv[])
{
	int fd, len;
	u_char buf[1024];
	mbus_frame reply;
	mbus_frame_data frame_data;
	char *xml_result = NULL;

	if (argc != 2)
    {
        fprintf(stderr, "usage: %s binary-file\n", argv[0]);
        return 1;
    }

	if ((fd = open(argv[1], O_RDONLY, 0)) == -1)
    {
		fprintf(stderr, "%s: failed to open '%s'", argv[0], argv[1]);
        return 1;
    }

	memset(buf, 0, sizeof(buf));
	len = read(fd, buf, sizeof(buf));

	close(fd);

	memset(&reply, 0, sizeof(reply));
	memset(&frame_data, 0, sizeof(frame_data));
	mbus_parse(&reply, buf, len);
	mbus_frame_data_parse(&reply, &frame_data);
	mbus_frame_print(&reply);
	
	if ((xml_result = mbus_frame_data_xml(&frame_data)) == NULL)
    {
        fprintf(stderr, "Failed to generate XML representation of MBUS frame: %s\n", mbus_error_str());
        return 1;
    }
    printf("%s", xml_result);
    free(xml_result);
	
	return 0;
}


