"""
Description of what this file does.
"""

import os
import sys
cur_path=os.path.abspath(os.path.dirname(__file__))
sys.path.insert(0, cur_path+"/..")

from src.utility.logger import *

import time

logging.debug('Beginning of code...')


class MyActuator:
    """ MyActuator connection manager. """
    def __init__(self, connection_string, *args, **kwargs):
        logging.debug('connecting to Motor on: %s', connection_string)

        logging.info('Motor connection successful')

    def __enter__(self):
        ''' Send regular heartbeats while using in a context manager. '''
        logging.info('__enter__ -> reviving heart (if required)')
        return self

    def __exit__(self, *args):
        ''' Automatically disarm and stop heartbeats on error/context-close. '''
        logging.info('__exit__ -> disarming, stopping heart and closing connection')
        # Close Motor connection
        logging.info('disconnect -> closing Motor connection') 
        self.master.close()

if __name__ == '__main__':
    # Set up option parsing to get connection string
    import argparse  
    parser = argparse.ArgumentParser(description='Description of what this file does.')
    parser.add_argument('--connect', help="Motor COM Port")
    args = parser.parse_args()

    connection_string = args.connect

    if not connection_string:
        connection_string = 'COM1'

    if not connection_string:
        logging.critical("No connection string specified, exiting code.")
        exit()


    with MyActuator(connection_string) as motor:
        logging.debug("Ready: %s", motor)

        def set_servo(motor_num, pwm_value):
            motor = motor

        logging.debug("Basic pre-arm checks")
