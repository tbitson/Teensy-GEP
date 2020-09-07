class AudioControlSGTL5000 : public AudioControl
{
public:
	AudioControlSGTL5000(void) : i2c_addr(0x0A) { }
	void setAddress(uint8_t level);
	bool enable(void);
	bool disable(void) { return false; }
	bool volume(float n) { return volumeInteger(n * 129 + 0.499); }
	bool inputLevel(float n) {return false;}
	bool muteHeadphone(void) { return write(0x0024, ana_ctrl | (1<<4)); }
	bool unmuteHeadphone(void) { return write(0x0024, ana_ctrl & ~(1<<4)); }
	bool muteLineout(void) { return write(0x0024, ana_ctrl | (1<<8)); }
	bool unmuteLineout(void) { return write(0x0024, ana_ctrl & ~(1<<8)); }
	bool inputSelect(int n) {
		if (n == AUDIO_INPUT_LINEIN) {
			return write(0x0020, 0x055) // +7.5dB gain (1.3Vp-p full scale)
			 && write(0x0024, ana_ctrl | (1<<2)); // enable linein
		} else if (n == AUDIO_INPUT_MIC) {
			return write(0x002A, 0x0173) // mic preamp gain = +40dB
			 && write(0x0020, 0x088)     // input gain +12dB (is this enough?)
			 && write(0x0024, ana_ctrl & ~(1<<2)); // enable mic
		} else {
			return false;
		}
	}
	bool volume(float left, float right);
	bool micGain(unsigned int dB);
	bool lineInLevel(uint8_t n) { return lineInLevel(n, n); }
	bool lineInLevel(uint8_t left, uint8_t right);
	unsigned short lineOutLevel(uint8_t n);
	unsigned short lineOutLevel(uint8_t left, uint8_t right);
	unsigned short dacVolume(float n);
	unsigned short dacVolume(float left, float right);
	bool dacVolumeRamp();
	bool dacVolumeRampLinear();
	bool dacVolumeRampDisable();
	unsigned short adcHighPassFilterEnable(void);
	unsigned short adcHighPassFilterFreeze(void);
	unsigned short adcHighPassFilterDisable(void);
	unsigned short audioPreProcessorEnable(void);
	unsigned short audioPostProcessorEnable(void);
	unsigned short audioProcessorDisable(void);
	unsigned short eqFilterCount(uint8_t n);
	unsigned short eqSelect(uint8_t n);
	unsigned short eqBand(uint8_t bandNum, float n);
	void eqBands(float bass, float mid_bass, float midrange, float mid_treble, float treble);
	void eqBands(float bass, float treble);
	void eqFilter(uint8_t filterNum, int *filterParameters);
	unsigned short autoVolumeControl(uint8_t maxGain, uint8_t lbiResponse, uint8_t hardLimit, float threshold, float attack, float decay);
	unsigned short autoVolumeEnable(void);
	unsigned short autoVolumeDisable(void);
	unsigned short enhanceBass(float lr_lev, float bass_lev);
	unsigned short enhanceBass(float lr_lev, float bass_lev, uint8_t hpf_bypass, uint8_t cutoff);
	unsigned short enhanceBassEnable(void);
	unsigned short enhanceBassDisable(void);
	unsigned short surroundSound(uint8_t width);
	unsigned short surroundSound(uint8_t width, uint8_t select);
	unsigned short surroundSoundEnable(void);
	unsigned short surroundSoundDisable(void);
	void killAutomation(void) { semi_automated=false; }