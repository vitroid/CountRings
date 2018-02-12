#!/usr/bin/env python3

from countrings import countrings_nx as cr
import sys
import argparse as ap
import logging



def getoptions():
    parser = ap.ArgumentParser(description='')
    parser.add_argument('--count', '-c', action='store_true', dest='countonly',
                        help='Only output the ring statistics.')
    parser.add_argument('maxsize', nargs='?', default=8, type=int, 
                       help='Maximum size of the ringd')

    parser.add_argument('--debug', '-D', action='store_true', dest='debug',
                        help='Output debugging info.')
    parser.add_argument('--quiet', '-q', action='store_true', dest='quiet',
                        help='Do not output progress messages.')
    return parser.parse_args()


def main():
    options = getoptions()
    if options.debug:
        logging.basicConfig(level=logging.DEBUG,
                            format="%(asctime)s %(levelname)s %(message)s")
    elif options.quiet:
        logging.basicConfig(level=logging.WARN,
                            format="%(asctime)s %(levelname)s %(message)s")
    else:
        #normal
        logging.basicConfig(level=logging.INFO,
                            format="%(asctime)s %(levelname)s %(message)s")
    logger = logging.getLogger()
    logger.debug("Debug mode.")
    logger.debug("Max size: {0}".format(options.maxsize))
    logger.debug("Stat only: {0}".format(options.countonly))
    
    
    file = sys.stdin
    while True:
        line = file.readline()
        if not line:
            break
        if line[0:5] == "@NGPH":
            (nmol,network) = cr.readNGPH(file)
            #print shortest_path(network, 0,3)
            ri = cr.CountRings(network).rings_iter( options.maxsize )
            if options.countonly:
                count = [0] * (options.maxsize-2)
                for ring in ri:
                    count[len(ring)-3]+=1
                print (" ".join([str(x) for x in count]) )
            else:
                print (cr.saveRNGS( nmol, ri ),end="")

if __name__ == "__main__":
    main()
