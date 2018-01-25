#ifndef MAP2D_WIDGET_H
#define MAP2D_WIDGET_H

#include <QGLWidget>
#include <QPainter>
#include "vsignal.h"

//=======================================================================================
class PainterSaver final
{
public:
    PainterSaver(QPainter * p_);
    ~PainterSaver();

private:
    QPainter *p;
};
//=======================================================================================
QColor color_alpha(Qt::GlobalColor gc, int alpha);
//=======================================================================================


//=======================================================================================
class Map2D_Widget : public QGLWidget
{
    Q_OBJECT

signals:
    void display_options_changed( const QPointF shifting,
                                  float zoom );

public:
    explicit Map2D_Widget(QWidget *parent = nullptr);

    //-----------------------------------------------------------------------------------

    // Перед масштабированием и изменением координат, т.е. в начале отрисовки и
    // после основной, масштабной, отрисовки, возвращается системная система и дается
    // возможность нанести конечные штрихи.
    VSignal<QPainter*,Map2D_Widget*> pre_paint_on_me;
    VSignal<QPainter*,Map2D_Widget*> post_paint_on_me;

    VSignal<QPainter*,Map2D_Widget*> paint_on_me;

    //-----------------------------------------------------------------------------------

    void set_black_std_preset();

    QPainter::RenderHints render_hint = QPainter::HighQualityAntialiasing |
                                        QPainter::TextAntialiasing;

    QBrush clear_brush = QBrush(Qt::white);
    QPen   clear_pen   = QPen(Qt::NoPen);

    QBrush entry_brush = QBrush(Qt::NoBrush);
    QPen   entry_pen   = QPen(Qt::black);


    //-----------------------------------------------------------------------------------

    bool need_paint_grid = true;

    QPointF scene_topleft      = QPointF( -3, -17 );
    QPointF scene_bottomright  = QPointF( 52,  12 );

    QPen pen_grid_1m  = QPen(Qt::gray,  1); //  Масштабируются при отрисовке.
    QPen pen_grid_5m  = QPen(Qt::gray,  2); //
    QPen pen_grid_10m = QPen(Qt::gray,  3); //
                                               //
    QPen pen_X_axis = QPen(Qt::black, 3);   //
    QPen pen_Y_axis = QPen(Qt::black, 3);   //

    //-----------------------------------------------------------------------------------
    QPen scaled_pen( QPen p ) const;
    QPen scaled_pen( Qt::GlobalColor c ) const          { return scaled_pen(QPen(c)); }

    void set_zoom( float zoom );

    //-----------------------------------------------------------------------------------
private:
    void paintEvent(QPaintEvent*) override;

    void _paint_grid( QPainter *p );
    void _paint_one_grid(QPainter *p, float increment );


    void mousePressEvent   (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;
    void mouseMoveEvent    (QMouseEvent *event) override;

    QPointF _pressed_point;
    bool    _moving_now    = false;
    QPointF _shifting      = QPointF(0,0);
    QPointF _move_shifting = QPointF(0,0);
    QPointF _shifting_sum() const;

    void wheelEvent(QWheelEvent*) override;
    float _zoom = 1;

    // Устанавливается в paintEvent как реальный масштаб относительно текущего окна.
    float native_scale() const;

    float x_len() const { return scene_bottomright.x() - scene_topleft.x(); }
    float y_len() const { return scene_bottomright.y() - scene_topleft.y(); }
};
//=======================================================================================



#endif // MAP2D_WIDGET_H
