LDFLAGS = -lwiringPi -lm -lpthread -lwiringPiPca9685
Spider: EncoderM.c
	gcc EncoderM.c -o EncoderTest $(LDFLAGS)
