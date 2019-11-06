from Crypto.Protocol.KDF import PBKDF2
from Crypto.Hash import SHA
from Crypto.Hash import SHA256
from Crypto.Hash import MD5
from Crypto.Hash import HMAC
from binascii import hexlify
import time
import string


# password 1 complexity = 26*26*...*26 = 26^6
password1 = "abcdef"
# password 2 complexity = 26^3
password2 = "ghi"

sha_hash = SHA.new()
sha_hash.update(password1)
#sha_key is SHA-1 hash of password 1
sha_key = sha_hash.hexdigest()

PBKDF2_key = PBKDF2(password2, "", dkLen=20, count=1000)
# PBKDF2_key is PBKDF2 protocol ran on password 2
PBKDF2_key = hexlify(PBKDF2_key)


print("-----------DEMO 1----------------------")
# two users have the same password
password = "kldjhf34127dh5"
# system salts both users passwords with pseudo-random value, stores salt in plaintext with hash
user1_salt = "dk32nd45"
user2_salt = "kjd832df"

sha_hash = SHA256.new()
sha_hash.update(password + user1_salt)
print("User 1 Hashed Password: {}".format(sha_hash.hexdigest()))

sha_hash = SHA256.new()
sha_hash.update(password + user2_salt)
print("User 2 Hashed Password: {}".format(sha_hash.hexdigest()))

#Results in both users have a different hash, forcing an attacker to brute force every
	# single user's password, regardless of past successes.

print("Press any key to continue")
raw_input()


print("-----------DEMO 2----------------------")
print("sha-hash: {}".format(sha_key))
print("PBKDF2: {}".format(PBKDF2_key))

alphabet = string.ascii_lowercase



def bruteforce1():
	start = time.time()
	for a1 in alphabet:
		for a2 in alphabet:
			for a3 in alphabet:
				for a4 in alphabet:
					for a5 in alphabet:
						for a6 in alphabet:
							pw = a1+a2+a3+a4+a5+a6
							h = SHA.new()
							h.update(pw)
							if h.hexdigest() == sha_key:
								print("Found Password!: {}".format(pw))
								print("time took: {}".format(time.time() - start))
								return

def bruteforce2():
	start = time.time()
	for a1 in alphabet:
		for a2 in alphabet:
			for a3 in alphabet:
				pw = a1+a2+a3
				h = PBKDF2(pw, "", dkLen=20, count=1000)
				h = hexlify(h)
				if h == PBKDF2_key:
					print("Found Password!: {}".format(pw))
					print("time took: {}".format(time.time() - start))
					return

bruteforce1()
bruteforce2()
