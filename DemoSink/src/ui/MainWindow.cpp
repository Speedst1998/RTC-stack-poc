
#include <QDebug>
#include <QtGui/QPainter>
#include <QTimer>
#include <libyuv.h>
#include "MainWindow.h"

#define vpx_dx_interface (vpx_codec_vp8_dx())

namespace {
    static const char* vpx_format_to_string(vpx_img_fmt_t f) {
        switch (f) {
        case VPX_IMG_FMT_I420: return "I420";
        case VPX_IMG_FMT_I422: return "I422";
        case VPX_IMG_FMT_I444: return "I444";
        case VPX_IMG_FMT_I440: return "I440";
        case VPX_IMG_FMT_YV12: return "YV12";
        case VPX_IMG_FMT_I42016: return "I42016";
        case VPX_IMG_FMT_I42216: return "I42216";
        case VPX_IMG_FMT_I44416: return "I44416";
        case VPX_IMG_FMT_I44016: return "I44016";
        default: return "Other";
        }
    }
    void printFmt(vpx_img_fmt_t fmt) {
        qDebug() << "Format : " << vpx_format_to_string(fmt); 
    }
}

MainWindow::MainWindow(QWidget *parent, const Qt::WindowFlags &flags)
    : QMainWindow(parent, flags), refresh_timer(new QTimer(this)) {
  vpx_codec_dec_cfg_t cfg;

  // Setting number of threads to a constant value (1)
  cfg.threads = 8;
  cfg.h = cfg.w = 0;  // set after decode

  auto err = vpx_codec_dec_init(&ctx, vpx_dx_interface, &cfg, VPX_CODEC_USE_FRAME_THREADING);

  setWindowTitle(QStringLiteral("Demo RTP"));
  resize(480, 275);

  refresh_timer->connect(refresh_timer, SIGNAL(timeout()), this, SLOT(repaint()));
  refresh_timer->setTimerType(Qt::PreciseTimer);
  refresh_timer->start(33);
}

MainWindow::~MainWindow() {
  vpx_codec_destroy(&ctx);
}

void MainWindow::NewSsrc(const uint32_t unknownSsrc) {

}

bool MainWindow::showVp8Frame(const std::unique_ptr<rtp::packetization::EncodedVideoFrame> &&frame) {
  auto err_decode = vpx_codec_decode(&ctx, frame->payload.data(), frame->payload.size(), NULL, 0);
  if (err_decode) {
    printf("Unable to decode frame!\n%s\n", vpx_codec_err_to_string(err_decode));
    return false;
  }

  /* An encoded image might contain more than one frame */
  vpx_codec_iter_t iter = nullptr;
  while (auto img = vpx_codec_get_frame(&ctx, &iter)) {
    if (image.width() != img->d_w || image.height() != img->d_h || image.format() != QImage::Format_RGB16) {
      qDebug() << "Recreating frame with size " << img->d_w << "x" << img->d_h;
      printFmt(img->fmt);
      image = QImage(img->d_w, img->d_h, QImage::Format_RGB16);
    }

    ConvertFromI420(
        img->planes[VPX_PLANE_Y], img->stride[VPX_PLANE_Y],
        img->planes[VPX_PLANE_U], img->stride[VPX_PLANE_U],
        img->planes[VPX_PLANE_V], img->stride[VPX_PLANE_V],
        image.bits(), img->d_w * 2,
        img->d_w, img->d_h,
        libyuv::FOURCC_RGBP
    );
  }

  return true;
}

void MainWindow::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.drawImage(0, 0, image);
}
