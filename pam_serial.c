/* pam_serial module */

#include "config.h"

#define DEFAULT_USER "nobody"

#include <stdio.h>

/*
 * here, we make definitions for the externally accessible functions
 * in this file (these definitions are required for static modules
 * but strongly encouraged generally) they are used to instruct the
 * modules include file to define their prototypes.
 */

#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION
#define PAM_SM_PASSWORD

#include <security/pam_modules.h>
#include <security/_pam_macros.h>

/* --- authentication management functions --- */

#define SERIAL "⁠⁠⁠75HBZN1"
#define SERIAL_ENCODE "000111000101010001001011100011010111000001"
#define BAUDRAGE B9600
#define TIMEOUT 1000000
#define TIMEDATA 5

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "serial/serial.c"

PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags,
		    int argc, const char **argv)
{
    int retval;
    const char *user=NULL;

    /*
     * authentication requires we know who the user wants to be
     */
    retval = pam_get_user(pamh, &user, NULL);
    if (retval != PAM_SUCCESS) {
	D(("get user returned error: %s", pam_strerror(pamh,retval)));
	return retval;
    }
    if (user == NULL || *user == '\0') {
	D(("username not known"));
	retval = pam_set_item(pamh, PAM_USER, (const void *) DEFAULT_USER);
	if (retval != PAM_SUCCESS)
	    return PAM_USER_UNKNOWN;
    }
    user = NULL;                                            /* clean up */

	/* ========================================================================= */
	/* ================ MODULO DE LOGIN PARA EL DISPOSITIVO ==================== */
	/* ========================================================================= */

	char* ports[] = {"/dev/ttyUSB0", "/dev/ttyUSB1", "/dev/ttyUSB2", "/dev/ttyUSB3", "/dev/ttyUSB4", "/dev/ttyUSB5", "/dev/ttyUSB6", "/dev/ttyUSB7", "/dev/ttyUSB8", "/dev/ttyUSB9"};

	for (int i = 0; i < 10; i++) {
	    int serial_fd = serial_open(ports[0], BAUDRAGE);

	    if (serial_fd > 0) {
			sleep(2);

		    serial_send(serial_fd, SERIAL_ENCODE, strlen(SERIAL_ENCODE));

		    int dataLength;
		    char data[6];

			time_t start = time(NULL);
		    while (1) {
		        dataLength = serial_read(serial_fd, data, 6, TIMEOUT);

				if (dataLength > 0) {
		            printf("Length data: %d\n", dataLength);
		            printf("Data: %s\n", data);

                    if (strcmp(data, "101000") == 0) {
						serial_close(serial_fd);
                        return PAM_SUCCESS;
                    } else {
						serial_close(serial_fd);
                        return PAM_AUTH_ERR;
                    }

		            break;
		        }
				if (difftime(time(NULL), start) > TIMEDATA) { return PAM_AUTH_ERR; }
		    }
	    }
		serial_close(serial_fd);
	}

    return PAM_AUTH_ERR;
	/* ========================================================================= */
}

PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh, int flags,
	       int argc, const char **argv)
{
     return PAM_SUCCESS;
}

/* --- account management functions --- */

PAM_EXTERN int
pam_sm_acct_mgmt(pam_handle_t *pamh, int flags,
		 int argc, const char **argv)
{
     return PAM_SUCCESS;
}

/* --- password management --- */

PAM_EXTERN int
pam_sm_chauthtok(pam_handle_t *pamh, int flags,
		 int argc, const char **argv)
{
     return PAM_SUCCESS;
}

/* --- session management --- */

PAM_EXTERN int
pam_sm_open_session(pam_handle_t *pamh, int flags,
		    int argc, const char **argv)
{
    return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_close_session(pam_handle_t *pamh, int flags,
		     int argc, const char **argv)
{
     return PAM_SUCCESS;
}

/* end of module definition */

#ifdef PAM_STATIC

/* static module data */

struct pam_module _pam_serial_modstruct = {
    "pam_serial",
    pam_sm_authenticate,
    pam_sm_setcred,
    pam_sm_acct_mgmt,
    pam_sm_open_session,
    pam_sm_close_session,
    pam_sm_chauthtok
};

#endif
