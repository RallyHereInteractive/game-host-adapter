import a2s
import logging

if __name__ == '__main__':
    logging.getLogger("a2s").setLevel(logging.DEBUG)
    info = a2s.info(('127.0.0.1', 23891))
    print(info)