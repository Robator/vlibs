/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  Authors:
**  Alexandre Gromtsev aka elapidae     elapidae@yandex.ru
**  Nadezhda Churikova aka claorisel    claorisel@gmail.com
**  Ekaterina Boltenkova aka kataretta  kitkat52@yandex.ru
**  Ivan Deylid aka sid1057             ivanov.dale@gmail.com>
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/


#ifndef VGSTPIPELINE_H
#define VGSTPIPELINE_H


// https://gstreamer.freedesktop.org/data/doc/gstreamer/1.9/gstreamer/html/GstPipeline.html


//GObject
// ╰── GInitiallyUnowned
//     ╰── GstObject
//         ╰── GstElement
//             ╰── GstBin
//                 ╰── GstPipeline

#include "vgstbin.h"


class VGstPipeline : public VGstBin
{
public:
    VGstPipeline();
};

#endif // VGSTPIPELINE_H
