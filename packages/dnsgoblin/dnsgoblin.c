/*******************************************************************************
 *                                                                             *
 *         ~    .__ °.__   0       o                    ^   .__ °__  `´        *
 *  °____) __ __|  | | °|   ______°____ 0 ____  __ _________|__|/  |_ ___.__.  *
 *  /    \|  | °\  |°|  | °/  ___// __ \_/ ___\|  | °\_  __ \ o\   __<   |  |  *
 * | o°|  \  |  /  |_|  |__\___ \\  ___/\ °\___| o|  /|  | \/  ||  |° \___ O|  *
 * |___|  /____/|____/____/____ °>\___  >\___  >____/ |__|° |__||__|  / ____|  *
 * `´´`´\/´`nullsecurity team`´\/`´´`´\/`´``´\/  ``´```´```´´´´`´``0_o\/´´`´´  *
 *                                                                             *
 * dnsgoblin.c - nasty creature constantly searching for DNS servers           *
 *                                                                             *
 * DATE                                                                        *
 * 03/11/2011                                                                  *
 *                                                                             *
 * DESCRIPTION                                                                 *
 * dnsgoblin uses standard dns querys and waits for the replies.               *
 *                                                                             *
 * AUTHOR                                                                      *
 * atzeton - http://www.nullsecurity.net/                                      *
 *                                                                             *
 * COMPILE                                                                     *
 * gcc dnsgoblin.c -O2 -lpthread -Wall -Wextra -pedantic \                     *
 * --std=gnu99 -D_REENTRANT                                                    *
 *                                                                             *
 * NOTES                                                                       *
 * This tool is for educational and testing purposes only.                     *
 * The author is NOT responsible for any kind of misusage!                     *
 * You may pipe stdout into a file: ./dnsgoblin > dnslist                      *
 *                                                                             *
 ******************************************************************************/


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <linux/ip.h>
#include <inttypes.h>
#include <pthread.h>


/* the ip header struct */
struct ipheader {
	uint8_t v;
	uint8_t tos;
	uint16_t len;
	uint16_t id;
	uint16_t off;
	u_char ttl;
	u_char p;
	uint16_t sum;
	uint32_t src;
	uint32_t dst;
}; 

/* _beginning_ of dns header */
struct dnsheader {
	uint16_t trans_id;
	/* incomplete */
};


void 	 sig_int(int sig);
char 	*human_addr(uint32_t ip_addr);
void 	*lstn(void *ptr);
int8_t   check_ip_addr(char *ptr);
int  	 main(int argc, char **argv);


/* quit if SIGINT is received */
void 
sig_int(int sig)
{
	if (sig != SIGINT) {
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}

/* check if the given ip is valid */
int8_t 
check_ip_addr(char *ptr)
{
	if( strlen(ptr) > 16) {
		printf("error: ip addr too long\n");
		exit(EXIT_FAILURE);
	}
	
	if( (int)inet_addr(ptr) == -1 ) {
		printf("error: ip addr not correct\n");
		exit(EXIT_FAILURE);
	}
	
	return(0);
}


/* int ip -> dotted decimals */
char *
human_addr(uint32_t ip_addr) 
{
	char *ptr    = calloc(1,16);
	uint8_t oct1 = 0;
	uint8_t oct2 = 0;
	uint8_t oct3 = 0;
	uint8_t oct4 = 0;;
	
	oct1 = ( ip_addr >> 24 ) & 0xFF;
	oct2 = ( ip_addr >> 16 ) & 0xFF;
	oct3 = ( ip_addr >> 8  ) & 0xFF;
	oct4 =   ip_addr         & 0xFF;
	
	sprintf(ptr,"%d.%d.%d.%d",oct4,oct3,oct2,oct1);
	return(ptr);
}


/* listen for dns responses */
void 
*lstn(void *ptr)
{
	int *sptr             = ptr;
	int sockfd            = *sptr;
	struct ipheader *ip   = NULL;
	uint8_t *pkt_recv     = calloc(1,2048);
	char *cptr            = NULL;
	
	while(1==1) {
		memset(pkt_recv,0x00,2047);
		
		if( recv(sockfd, pkt_recv, 2047, 0) > 0) {
			ip = (struct ipheader *)pkt_recv;
			cptr = human_addr(ip->src);
			printf("%s\n",cptr);
			free(cptr);
		}
	}
	
}
	

int main(int argc, char **argv)
{	
	int32_t sockfd;			/* raw socket to inject packets */
	int32_t sockfd2;		/* prevent ICMP port unreach msgs creating a layer4 udp sock on iface */
	
	struct sockaddr_in ifcfg;
	struct sockaddr_in sin;
	
	int        one = 1;
    const int *val = &one;
    
    /* what can you find out here? */
    char packet[]  = "\x45\x00\x00\x3b\x6c\xbb\x40\x00\x40\x11\x6b\xd4\xc0\xa8\x02\x65\x55\xd6\x49\x3f\x13\x37\x00\x35\x00\x27\x00\x00\xd6\x88\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\x03\x77\x77\x77\x06\x61\x6d\x61\x7a\x6f\x6e\x02\x64\x65\x00\x00\x01\x00\x01";

	struct ipheader  *ip  = NULL;
	struct udphdr    *udp = NULL;
	struct dnsheader *dns = NULL;
	
	uint16_t local_port   = 0;
	pthread_t trd_lstn;
	
	if( argc < 2) {
		printf("error: need local ip as arg\n");
		exit(EXIT_FAILURE);
	}	
	
	check_ip_addr(argv[1]);
	
	srand ( time(NULL)    );
	signal(SIGINT, sig_int); /* set up a signal handler */

	
	/* get random port */
	local_port = (uint32_t)rand();
	
	/* pseudo socket */
	sockfd2 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* bind to port (used to prevent icmp bla) */
	ifcfg.sin_family      = AF_INET;
	ifcfg.sin_port        = htons(local_port);
	ifcfg.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(sockfd2, (struct sockaddr *)&ifcfg, sizeof(ifcfg));
	
	/* create main raw sock */
	sockfd         = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	sin.sin_family = AF_INET;
	sin.sin_port   = htons (local_port);

    if (setsockopt (sockfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0) {
		printf("err\n");
	}

	if(sockfd < 0) {
		fprintf(stderr,"error: rawsock cannot be created. No permission.\n");
		exit(EXIT_FAILURE);
	}
	
	/* change the UDI (-> nobody) */
	setuid(65534);
	
	/* create thread which is listening for dns responses */
	pthread_create(&trd_lstn, NULL, lstn, (void *)&sockfd);
	
	ip = (struct ipheader *)packet;
	ip->id  = ntohs( rand() );
	ip->v   = 0x45;
	ip->tos = 0x0054;
	ip->len = 30;
	ip->off = 0x0000;
	ip->ttl = 0xff;
	ip->p   = 17;
	ip->sum = 0x0000;
	
	ip->src = inet_addr(argv[1]);
	
	udp         = (struct udphdr *)(packet + sizeof(struct ipheader));
	udp->source = ntohs( local_port );
	udp->dest   = ntohs(53);
	
	dns = (struct dnsheader *)(packet + sizeof(struct ipheader) + sizeof(struct udphdr) );
	
	while(1==1) {
		/* do some modifications */
		ip->id  = ntohs( rand() );
		ip->dst = ntohl( rand() );

		dns->trans_id = ntohs( rand() );
		
		if( sendto(sockfd, &packet, sizeof(packet)-1 , 0, (struct sockaddr *) &sin, sizeof (sin)) < 0){
			printf("error: sendto failed\n");
			exit(EXIT_FAILURE);
		}
		
		usleep(5000); /* you may increase/decrease this */
	}

	close(sockfd);

	return 0;
}

/* EOF */
