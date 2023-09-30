#include <iostream>
#include <vector>
#include <cmath>
#include <sndfile.h>

// Function to calculate L2 norm (average mean squared error) between 2 vectors
// nao sei se é double ou int??
double calculateL2Norm(const std::vector<double>& original, const std::vector<double>& modified) {
    double sum = 0.0;
    size_t numSamples = original.size();

    for (size_t i = 0; i < numSamples; ++i) {
        double diff = original[i] - modified[i];
        sum += diff * diff;
    }

    return std::sqrt(sum / numSamples);
}

// Function to calculate L∞ norm (maximum per sample absolute error) between 2 vectors
double calculateLInfNorm(const std::vector<double>& original, const std::vector<double>& modified) {
    double maxAbsError = 0.0;
    size_t numSamples = original.size();

    for (size_t i = 0; i < numSamples; ++i) {
        double absError = std::abs(original[i] - modified[i]);
        if (absError > maxAbsError) {
            maxAbsError = absError;
        }
    }

    return maxAbsError;
}

// Function to calculate signal-to-noise ratio (SNR) between 2 vectors
double calculateSNR(const std::vector<double>& original, const std::vector<double>& modified) {
    double signalPower = 0.0;
    double noisePower = 0.0;
    size_t numSamples = original.size();

    for (size_t i = 0; i < numSamples; ++i) {
        signalPower += original[i] * original[i];
        noisePower += (original[i] - modified[i]) * (original[i] - modified[i]);
    }

    return 10 * std::log10(signalPower / noisePower);
}

int main(int argc, char* argv[]) {
    // check for nº of arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <original.wav> <modified.wav>" << std::endl;
        return 1;
    }

    // Open the original audio file
    SF_INFO originalInfo;
    SNDFILE* originalFile = sf_open(argv[1], SFM_READ, &originalInfo);
    if (!originalFile) {
        std::cerr << "Error: Cannot open original audio file" << std::endl;
        return 1;
    }

    // Open the modified audio file
    SF_INFO modifiedInfo;
    SNDFILE* modifiedFile = sf_open(argv[2], SFM_READ, &modifiedInfo);
    if (!modifiedFile) {
        std::cerr << "Error: Cannot open modified audio file" << std::endl;
        sf_close(originalFile);
        return 1;
    }

    // Check if both files have the same sample rate and number of channels
    if (originalInfo.samplerate != modifiedInfo.samplerate || originalInfo.channels != modifiedInfo.channels) {
        std::cerr << "Error: Original and modified audio files must have the same sample rate and number of channels" << std::endl;
        sf_close(originalFile);
        sf_close(modifiedFile);
        return 1;
    }

    // Read audio data from both files
    const size_t numSamples = originalInfo.frames * originalInfo.channels;
    std::vector<double> originalData(numSamples);
    std::vector<double> modifiedData(numSamples);

    sf_readf_double(originalFile, originalData.data(), numSamples);
    sf_readf_double(modifiedFile, modifiedData.data(), numSamples);

    // Calculate and print L2 norm for each channel and average
    for (int channel = 0; channel < originalInfo.channels; ++channel) {
        std::vector<double> originalChannelData;
        std::vector<double> modifiedChannelData;

        for (size_t i = channel; i < numSamples; i += originalInfo.channels) {
            originalChannelData.push_back(originalData[i]);
            modifiedChannelData.push_back(modifiedData[i]);
        }

        double l2Norm = calculateL2Norm(originalChannelData, modifiedChannelData);
        double lInfNorm = calculateLInfNorm(originalChannelData, modifiedChannelData);
        double snr = calculateSNR(originalChannelData, modifiedChannelData);

        std::cout << "Channel " << channel + 1 << ":" << std::endl;
        std::cout << "L2 Norm: " << l2Norm << std::endl;
        std::cout << "LInf Norm: " << lInfNorm << std::endl;
        std::cout << "SNR: " << snr << " dB" << std::endl;
    }

    // Calculate and print average L2 norm and SNR across all channels
    double totalL2Norm = 0.0;
    double totalSNR = 0.0;

    for (int channel = 0; channel < originalInfo.channels; ++channel) {
        std::vector<double> originalChannelData;
        std::vector<double> modifiedChannelData;

        for (size_t i = channel; i < numSamples; i += originalInfo.channels) {
            originalChannelData.push_back(originalData[i]);
            modifiedChannelData.push_back(modifiedData[i]);
        }

        totalL2Norm += calculateL2Norm(originalChannelData, modifiedChannelData);
        totalSNR += calculateSNR(originalChannelData, modifiedChannelData);
    }

    double averageL2Norm = totalL2Norm / originalInfo.channels;
    double averageSNR = totalSNR / originalInfo.channels;

    std::cout << "Average L2 Norm across all channels: " << averageL2Norm << std::endl;
    std::cout << "Average SNR across all channels: " << averageSNR << " dB" << std::endl;

    // Close the files
    sf_close(originalFile);
    sf_close(modifiedFile);

    return 0;
}