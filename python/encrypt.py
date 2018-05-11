# -*-encoding:utf-8-*-
import sys
from Crypto.Cipher import AES
from binascii import b2a_hex, a2b_hex
import base64

key = "whistle2016synce"

def encrypt(text):
    cryptor = AES.new(key, AES.MODE_CBC, key)
    length = 16
    count = len(text)
    add = length - (count % length)
    text = text + ('\0' * add)
    ciphertext = cryptor.encrypt(text)
    return b2a_hex(ciphertext)


def decrypt(text):
    cryptor = AES.new(key, AES.MODE_CBC, key)
    plain_text = cryptor.decrypt(a2b_hex(text))
    return plain_text  # .rstrip('\0')

if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf-8')

    text='hello world! this is a test text!'

    encode=encrypt(text)
    secret_text=base64.b64encode(encode)
    print secret_text
    decode_text=base64.b64decode(secret_text)
    orgtext = decrypt(decode_text);
    print orgtext
