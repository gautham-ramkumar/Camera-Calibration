#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    // Collect filenames
    std::vector<cv::String> files;
    cv::glob("/home/andal/Camera_Calibration/calibration_images/*.JPEG", files);
    if (files.empty()) {
        std::cerr << "No images found – check your path & extension\n";
        return -1;
    }

    // Chessboard parameters (inner corners per row & col) and square size
    cv::Size boardSize(6, 8);
    float squareSize = 0.01f;  // in meters (10mm)

    // Data containers
    std::vector<std::vector<cv::Point2f>> imagePoints;
    std::vector<std::vector<cv::Point3f>> objectPoints;
    cv::Size imageSize;  // will be set on first valid image

    // Pre‑compute the 3D corner template once
    std::vector<cv::Point3f> objTemplate;
    objTemplate.reserve(boardSize.area());
    for (int r = 0; r < boardSize.height; ++r)
        for (int c = 0; c < boardSize.width; ++c)
            objTemplate.emplace_back(c * squareSize,
                                     r * squareSize,
                                     0.0f);

    int flags = cv::CALIB_CB_NORMALIZE_IMAGE
              | cv::CALIB_CB_EXHAUSTIVE
              | cv::CALIB_CB_ACCURACY;

    // Loop over images: detect corners and collect correspondences
    for (auto &f : files) {
        cv::Mat img = cv::imread(f);
        if (img.empty()) {
            std::cerr << "Failed to load " << f << "\n";
            continue;
        }

        // capture image size once
        if (imageSize == cv::Size())
            imageSize = img.size();

        cv::Mat gray;
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Point2f> corners;
        bool found = cv::findChessboardCornersSB(gray, boardSize, corners, flags);
        if (!found) {
            std::cout << "No corners in " << f << "\n";
            continue;
        }

        // refine corner locations
        cv::cornerSubPix(
            gray, corners,
            cv::Size(11,11), cv::Size(-1,-1),
            cv::TermCriteria(
                cv::TermCriteria::EPS + cv::TermCriteria::COUNT,
                30, 0.001
            )
        );

        // store for calibration
        imagePoints.push_back(corners);
        objectPoints.push_back(objTemplate);

        // draw & display
        cv::drawChessboardCorners(img, boardSize, corners, found);
        cv::imshow("Detected Corners", img);
        cv::waitKey(0);
    }
    cv::destroyAllWindows();

    // Check we have enough views
    if (imagePoints.size() < 3) {
        std::cerr << "Not enough valid views (got "
                  << imagePoints.size() << ", need ≥3)\n";
        return -1;
    }

    // Perform calibration
    cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
    cv::Mat distCoeffs   = cv::Mat::zeros(8, 1, CV_64F);
    std::vector<cv::Mat> rvecs, tvecs;

    double rms = cv::calibrateCamera(
        objectPoints,
        imagePoints,
        imageSize,
        cameraMatrix,
        distCoeffs,
        rvecs,
        tvecs,
        0,  // flags if you want to fix additional params
        cv::TermCriteria(
            cv::TermCriteria::EPS + cv::TermCriteria::COUNT,
            30, 1e-6
        )
    );

    std::cout << "\nCalibration complete\n"
              << "RMS reprojection error = " << rms << "\n\n"
              << "Camera matrix:\n"    << cameraMatrix   << "\n\n"
              << "Distortion coeffs:\n" << distCoeffs     << "\n\n";

    // Undistort a sample image
    cv::Mat map1, map2, undist, sample = cv::imread(files[0]);
    cv::initUndistortRectifyMap(
        cameraMatrix, distCoeffs,
        cv::Mat(), cameraMatrix,
        imageSize, CV_16SC2, map1, map2
    );
    cv::remap(sample, undist, map1, map2, cv::INTER_LINEAR);

    cv::imshow("Original", sample);
    cv::imshow("Undistorted", undist);
    cv::waitKey(0);

    return 0;
}
