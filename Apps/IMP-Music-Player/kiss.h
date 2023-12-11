






    /// ##########################################################################################
    /// ############# SPECTRUM: (USING KissFFT LIBRARY) ##########################################
    /// ##########################################################################################

    /*
    static double xAxis = 0;
    static int sampleCount = 0;
    static double pieSteps;
    static double fullSinWave = 3.14159265*2;
    static double sampleRate = 44100;
    static double wantedHz = 0;
    int octiveOffset;
    int dataStream1[1024] = {0};
    kiss_fft_cpx dataStream2[1024] = {0};


    void test_FFT() {

    // for (int i=0; i<1024; i++){ dataStream1[i].i = dataStream1[i].r = 0; };

    for (int i=0; i<1024; i++) {
    wantedHz = 1000;
    pieSteps = fullSinWave/(sampleRate/wantedHz);
    xAxis += pieSteps;
    dataStream1[i] = (int)(sin(xAxis)*32768.0);
    // std::cout << "data: " << dataStream1[j] << std::endl;
    };

    kiss_fft_cfg mycfg = kiss_fft_alloc(1024, 0, NULL, NULL);
    kiss_fft_cpx* in_buf  = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx)*1024);
    kiss_fft_cpx* out_buf = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx)*1024);
    for (int i=0; i<1024; i++){
    in_buf[i].r = dataStream1[i];
    in_buf[i].i = dataStream1[i];
    };

    kiss_fft(mycfg, in_buf, out_buf);

    for (int i=0; i<256; i++){
    int ix = out_buf[i].i;
    int rx = out_buf[i].r;
    // printfbar(sqrt(ix*ix+rx*rx)););
    std::string mybar = "";
    for(int i=0; i<((sqrt(ix*ix+rx*rx))/2000); i++){ mybar += "*"; };
    /// std::cout << "Bar: " << mybar << '\r';
    std::cout << "Bar: " << mybar << std::endl;
    /// usleep(100);
    };

    };
    */



    /*
    int file_channels = 0;
    int file_rate = 0;
    int file_number_of_frames = 0; /// ?...
    audiere::SampleFormat file_format;
    audiere::SampleSource* source;
    size_t done = 0;

    int dataStream1[1024] = {0};
    */



    void testing_KissFFT() {


    /*

    std::string pa = "C://BACKUPS - IMPORTANTES - 01/- IRRLICHT/Irrlicht-Music-Player-V.1.5.006/bin/music/aateste.wav";
    source = audiere::OpenSampleSource(pa.c_str());



    /// short int* buffer = (short int*)malloc(source->getLength());
    // Get 2048 samples
    // const int TIME = 2048;
    unsigned char* buffer = new unsigned char[FFT_LEN];

    source->getFormat(file_channels, file_rate, file_format);
    SampleBuffer* sample_buffer = CreateSampleBuffer(buffer, file_number_of_frames, file_channels, file_rate, SF_ML16);

    // SampleSource* sample_source = audiereAudioDevice.get().openBuffer();
    // unsigned char* buffer = audiereAudioDevice->openBuffer();

    // unsigned char* buffer = CreateSampleBuffer(buffer, file_number_of_frames, file_channels, file_rate, SF_ML16);







    char *music_file = "C:/BACKUPS - IMPORTANTES - 01/- IRRLICHT/Irrlicht-Music-Player-V.1.5.006/bin/music/Gentle Moments.mp3";
    //int          readcount;

    short buf[WIN * 2];
    int nfft = WIN;
    double intensity = 0;
    kiss_fft_cfg cfg;
    kiss_fft_cpx cx_in[WIN];
    kiss_fft_cpx cx_out[WIN];
    short *sh;

    cfg = kiss_fft_alloc(nfft, 0, 0, 0);


    if (sf_open(music_file, SFM_READ, &sfinfo))
    {   // Open failed so print an error message.
        printf("Not able to open input file %s.\n", "input.wav");
        // Print the error message fron libsndfile.
        sf_perror(NULL);
        return  1;
    }

    while ((sf_read_short(infile, buf, WIN)))//fread(buf, 1, WIN * 2, in)
    {
        //system("cls");

        for (int i = 0;i<WIN;i++) {
            sh = (short *)&buf[i * 2];
            cx_in[i].r = (float) (((double)*sh) / 32768.0);
            cx_in[i].i = 0.0;
        }

        kiss_fft(cfg, cx_in, cx_out);
        //Display the value of a position
        int position = 511;
        intensity = sqrt(pow(cx_out[position].r, 2) + pow(cx_out[position].i, 2));
        printf("%9.4f\n", intensity);

        //Display all values
        for (i = 0;i<WIN;i++) {
            //printf("Joe: cx_out[i].r:%f\n", cx_out[i].r);
            //printf("Joe: cx_out[i].i:%f\n", cx_out[i].i);
            intensity = sqrt(pow(cx_out[i].r,2) + pow(cx_out[i].i,2));
            printf("%d - %9.4f\n", i, intensity);
        }

    }
    sf_close(infile);
    free(cfg);
    int temp;
    scanf_s("%d", &temp);

    */





    /*
    for (int i=0; i<FFT_LEN; i++) {
    std::cout << "L BUFFER: " << buffer[i] << std::endl;
    };
    */



    /*

    // 16-bit integer encoded in bytes, hence x2 size

     // Get 1024 samples
    const int TIME = 1024;


    short* samples = new short[done/2];
    int index = 0;

    // Iterate 2 bytes at a time
    for (int i=0; i<done; i+=2) {
    unsigned char first  = buffer[i];
    unsigned char second = buffer[i+1];
    samples[index++] = (first|(second<<8));
    };


    std::cout << "L BUFFER: " << buffer[0] << " - " << "R BUFFER: " << buffer[0+1] << std::endl;






    kiss_fft_cfg ComplexCfg;

    float        RealIn[FFT_LEN];
    kiss_fft_cpx ComplexIn[FFT_LEN];

    kiss_fft_cpx RealOut[FFT_LEN];
    kiss_fft_cpx ComplexOut[FFT_LEN];

    ComplexCfg = kiss_fft_alloc(FFT_LEN, 0, 0, 0);

    for (int i=0;i<FFT_LEN;i++) {
    RealIn[i]=cos( NUM*i*2*M_PI/FFT_LEN);
    ComplexIn[i].r=RealIn[i];
    ComplexIn[i].i=0;
    };

    kiss_fft(ComplexCfg, ComplexIn, ComplexOut);

    std::cout.precision(0);
    cout << "Real values:    " << std::fixed;
    for (int i=0;i<16;i++){ cout << sqrt(SQ(RealOut[i].r)+SQ(RealOut[i].i)) << " "; };
    cout << endl;

    cout << "Complex values: ";
    for (int i=0;i<16;i++){ cout << sqrt(SQ(ComplexOut[i].r)+SQ(ComplexOut[i].i)) << " "; };
    cout << endl;

    float* output = new float[FFT_LEN/2+1];

    for (int i=0; i<(FFT_LEN/2+1); i++) {
    float re = ComplexOut[i].r * FFT_LEN;
    float im = ComplexOut[i].i * FFT_LEN;
    output[i] = sqrtf(re*re + im*im);
    };


    for (int i=0; i<200; i++){ cout << output[i] << endl; };
    */







    /*

    // Array to store the calculated data
    int speclen = TIME/2+1;
    float* output = new float[speclen];

    // kiss_fftr_cfg config;
    kiss_fft_cfg config = kiss_fft_alloc(TIME, 0, NULL, NULL);
    /// kiss_fft_cpx* spectrum = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * TIME);


    kiss_fft_cpx* in_buf  = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx)*TIME);
    kiss_fft_cpx* out_buf = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx)*TIME);


    for (int i=0; i<TIME; i++){
    in_buf[i].r = buffer[i];
    in_buf[i].i = buffer[i];
    std::cout << "BUFFER: " << in_buf[i].r << std::endl;
    };


    kiss_fft(config, in_buf, out_buf);

    for (int i=0; i<256; i++){
    int ix = out_buf[i].i;
    int rx = out_buf[i].r;
    // printfbar(sqrt(ix*ix+rx*rx)););
    std::string mybar = "";
    for(int i=0; i<((sqrt(ix*ix+rx*rx))/2000); i++){ mybar += "*"; };
    /// std::cout << "Bar: " << mybar << '\r';
    std::cout << "Bar: " << mybar << std::endl;
    /// usleep(100);
    };
    */

    /*
    // Right here...
    kiss_fft(config, (kiss_fft_cpx*)samples, spectrum);
    for (int i=0; i<speclen; i++) {
    float re = scale(spectrum[i].r) * TIME;
    float im = scale(spectrum[i].i) * TIME;
    output[i] = sqrtf(re*re + im*im);
    };

    for (int i=0; i<256; i++){
    int ix = spectrum[i].i;
    int rx = spectrum[i].r;
    // printfbar(sqrt(ix*ix+rx*rx)););
    std::string mybar = "";
    for(int i=0; i<((sqrt(ix*ix+rx*rx))/2000); i++){ mybar += "*"; };
    /// std::cout << "Bar: " << mybar << '\r';
    std::cout << "Bar: " << mybar << std::endl;
    /// usleep(100);
    };
    */


    };




    /// ##########################################################################################
    /// ############# SPECTRUM: (USING KissFFT LIBRARY) ##########################################
    /// ##########################################################################################



