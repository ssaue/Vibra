<CsoundSynthesizer>
<CsOptions>
</CsOptions>
<CsInstruments>

	sr = 48000
	ksmps = 256
	nchnls = 1	
	0dbfs = 1

; Global variables

		instr 1
		OSCsend	1, "127.0.0.1", 32000, "/vibra/connect", "si", "127.0.0.1", 12000
		turnoff
		endin
        
        
</CsInstruments>
<CsScore>

i1	0.0 100
e

</CsScore>
</CsoundSynthesizer>

