=begin

= SDL

この拡張ライブラリのインターフェースはSDL本体と非常に似ています。
よってSDLのドキュメントが非常に参考になるでしょう。

すべてのクラス/モジュールはmodule SDLの下にあります。
このモジュール内では数はほぼすべて0から始まります。

== クラス/モジュール構成
* ((<Error>))
* ((<Surface>))
  * ((<Screen>))
* ((<PixelFormat>))
* ((<Event>))
* ((<Key>)) (module)
* ((<Mouse>)) (module)
* ((<Mixer>)) (module)
* ((<Mixer::Wave>))
* ((<Mixer::Music>))
* ((<WM>)) (module)
* ((<CD>))
* ((<Joystick>))
* ((<TTF>))

== Error

エラー通知用のクラスです。SDLモジュール内で生じるエラーほぼすべて
このクラスを用いて通知されます。

=== スーパークラス

StandardError

== 初期化関連

=== SDL内のモジュール関数

--- init(flag)
      SDLを初期化する。flagとして与えられる定数は以下のとおり。
        INIT_AUDIO
        INIT_VIDEO
        INIT_CDROM
        INIT_JOYSTICK

--- quit
      at_exit{ SDL.quit; } というように使ってください。

      version 0.3にて廃止。ライブラリから自動的によびだされる。

--- initializedSystem(flag)
      Not documented yet

== video関連

一部の機能は、SGEライブラリやSDL_imageが必要である。

bpp=bit per pixelである。

=== SDL内のモジュール関数

--- getVideoSurface
      Not documented yet

--- setVideoMode(w,h,bpp,flags)
      指定された画面の幅、高さ、bppの値でビデオモードを設定する。
      bppが0の場合、現在のディスプレイのbppの値が使用される。
      成功したときは((<Screen>))のオブジェクトを返す。
      失敗したときはSDL::Error例外が生じる。
      flagsの意味は以下のとおり。
      * SWSURFACE
        
        システムのメモリ内にバッファをとる。

      * HWSURFACE

        ビデオメモリにバッファをとる。

      * FULLSCREEN
        
        フルスクリーンモードで動作しようとする。

      * DOUBLEBUF

        ダブルバッファリングができるようにする。
        Screen#flipをよびだすことによってバッファの切り替えができる。

      * ANYFORMAT

        SDLライブラリは要求されたbppでの設定を試みるが、それと異なっても利用
        可能なモードがあればそれを返してくる。デフォルトの動作では、要求され
        たモードが直接サポートされていない場合はエミュレーションを行う。

      flagはそのほかにもある。さらに詳しく知る必要があれば
      SDLのドキュメントを見てください。

--- checkVideoMode(w,h,bpp,flags)
      指定されたビデオモードがサポートされているかどうかを調べる。
      指定サイズのスクリーンがどんなデプスでもサポートされていない場合は
      0を返し、いくつかのデプスで指定されたサイズがサポートされていれば、
      引数で指定したものに最も近いbppの値を返す。もしこの値がビデオモード
      を設定する際に指定したものと違っている場合は、setVideoModeは
      成功するが、指定されたモードはシャドウサーフェスを使ってエミュレー
      トされることになる。
      
      checkVideoModeに対する引数は、setVideoMode()で使用するもの
      と同じである。

--- listModes(flags)
      利用可能な解像度を返す。
      利用可能な解像度が存在しないときはnil返す。またあらゆる解像度が使用可
      能なときはtrueを返す。利用可能な解像度が1個以上あるときはその解像度を
      の横方向、縦方向の解像度の値をおさめた配列が1個以上入っている配列を返す。
      
      引数のflagは、setVideoModeで使用するものと同じである。

--- setGamma(redgamma,greengamma,bluegamma)
      ガンマを設定します。
      ガンマはスクリーン上での色の明るさやコントラストを調節します。
      r,g,bそれぞれのガンマ値は1.0で無調整と同等になります。

--- autoLock
      SGEが必要

      サーフィスのロックが必要なとき自動的にロックしてくれるかどうか
      を返す。デフォルトはtrue。

      さらに詳しいことを知りたければ((<Surface::lock>))を見てください。

--- autoLock=(autolocking)
      SGEが必要
      サーフィスのロックが必要なとき自動的にロックしてくれるように設定する。
      
--- videoInfo
      Videoの情報をVideoInfoのインスタンスで返す。その内容は以下の通り。
      真偽値を表すものはtrue,falseが入っている。
      詳しい意味はSDLのドキュメントを参照してください。
        VideoInfo#hw_available
        VideoInfo#wm_available
        VideoInfo#blit_hw
        VideoInfo#blit_hw_CC
        VideoInfo#blit_hw_A
        VideoInfo#blit_sw
        VideoInfo#blit_sw_CC
        VideoInfo#blit_sw_A
        VideoInfo#blit_fill
        VideoInfo#video_mem
        VideoInfo#vfmt
	
--- blitSurface(src,srcX,srcY,srcW,srcH,dst,dstX,dstY)
      srcで指定されたSurfaceからdstで指定されたSurfaceへの高速なblit
      を行う。

      srcX,srcY,srcW,srcHにすべて0を指定した場合はsrc全体をblitする。

      失敗時には例外SDL::Errorが生じる。      
      
      ロックしたサーフェスに対してこれをつかってはいけない。

--- blitSurface2(src,srcRect,dst,dstRect)
      blitSurface2(src,[0,32,32,32],dst,[100,200])
      のように使う。

      srcRectにnilを指定した場合はsrc全体をblitする。

--- rotateXYScaled(src,dst,x,y,angle,xscale,yscale)
      SGEが必要

      SGEのsge_rotate_xyscaled関数とはsrcとdstの順序が入れ替わっていることに
      注意するように。下の2つの関数も同様である。
      これは、blitSurfaceに合わせたためである。
      また、この仕様は変更する可能性がある。

      また、ColorKeyは無視される。

--- rotateScaled(src,dst,x,y,angle,scale)
      rotateXYScaledと同様、ただしxscaleとyscaleがともにscaleであると
      する。

--- rotate(src,dst,x,y,angle)
      rotateXYScaledと同様、ただしxscaleとyscaleがともに1であると
      する。

--- rotateScaledBlit(src,dst,x,y,angle,scale)
      SGEが必要
      ColorKeyは有効となる。
      rotateBlitも同様である。

--- rotateBlit(src,dst,x,y,angle)
      rotateScaledBlitと同様、ただしscaleが1であるとする。

=== Surface

画像を保持するクラスです。

==== スーパークラス

Object

==== クラスメソッド

--- Surface.new(flag,w,h,format)
      Not documented yet

--- Surface.loadBMP(filename)
      指定されたファイル名のWindows BMP形式のファイルから((<Surface>))
      のインスタンスを作成する。
      ファイルが開けない等のエラーが生じた場合はSDL::Error例外が生じる。

--- Surface.load(filename)
      SDL_imageが必要

      指定されたファイル名の画像ファイルから((<Surface>))
      のインスタンスを作成する。
      BMP,PPX,XPM,PCX,GIF,JPEG,PNG,TGAなどをロードできる。

==== メソッド

--- Surface#displayFormat
      selfをビデオフレームバッファのピクセルフォーマットと色に合わせた
      新しいサーフェスにコピーして返す。
      新しいsurfaceを使うことによってscreenへの高速なblitができる。
      
      カラーキーとα値に対するblitのアクセラレーションを利用したいのならば、
      この関数の呼び出しに先立ってカラーキーとα値の設定を行っておく
      必要がある。

--- Surface#setColorKey(flag,key)
      blit可能なサーフェスに対してカラーキー(透明なピクセル)を設定する。

      flagにSDL::SRCCOLORKEYを指定すると、そのピクセルはblitでsrcになる
      ほうの透明ピクセルを示していることになる。
      通常はこのフラグを使う。
      
      flagにSDL::RLEACCELとORをとって指定すると、RLEを使って高速なblitを
      しようとする。

      flagが0の場合は、color keyをクリアする。

--- Surface#fillRect(x,y,w,h,color)
      指定された長方形の領域をcolorでぬりつぶす。

--- Surface#setClipRect(x,y,w,h)
      クリッピングをする長方形を指定する。
      ((<blitSurface>)),((<Surface#put>))等でこのインスタンスに描画しようと
      したとき、これで指定した長方形内部のみで描画される。
      
--- Surface#setAlpha(flag,alpha)
      アルファの設定をする。
      flagにSDL::SRCALPHAを設定することでアルファが有効になる。
      SDL::RLEACCELとORをとって指定するとRLEによる高速化が有効になる。
      alphaはアルファ値を意味し、0で透明、255で通常と同等となる。

      flagに0を指定すればアルファは無効となる。

--- Surface#h
      selfの高さを返す。

--- Surface#w
      selfの横幅を返す。

--- Surface#format
      selfのpixel formatを返す。
      詳しくは((<PixelFormat>))を参照。

--- Surface#put(image,x,y)
      selfの位置(x,y)にimageを描画する。
      blitSurfaceで実装されている。

--- Surface#lock
      getPixel,setPixelやdrawLine等のメソッドを使って描画する
      ための準備をする。

      ((<Surface#mustLock?>))がtrueを返したインスタンスのみこの操作
      が必要となる。

      ((<Surface#unlock>))を呼びだすまで、他ライブラリの呼びだしや
      OSに対する操作をしてはならない。

      ((<autoLock>))がtrueを返すときはライブラリが自動的にこの操作を
      してくれるのでこのメソッドを呼びだす必要はない。

--- Surface#unlock
      ((<Surface#lock>))でロックしたのを解除する。

--- Surface#mustLock?
      ((<Surface#lock>))を呼びだす必要があるときはtrueを、
      ないときはfalseを返す。

--- Surface#getPixel(x,y) 
--- Surface#[](x,y)
      SGEが必要 ロックが必要

      x,yの位置のピクセルの値を返す。

--- Surface#putPixel(x,y,pixel)
--- Surface#[]=(x,y,pixel)
      SGEが必要 ロックが必要

      x,yの位置のピクセルの値をpixelにする。
      つまり、x,yの位置にpixelの色の点を打つ。

--- Surface#drawLine(x1,y1,x2,y2,color)
      SGEが必要 ロックが必要

      色がcolorの線を(x1,y1)から(x2,y2)まで描く。

--- Surface#drawRect(x,y,w,h,color)
      SGEが必要 ロックが必要

      色がcolorの長方形を描く。中はぬりつぶさない。

--- Surface#drawCircle(x,y,r,color)
      SGEが必要 ロックが必要

      色がcolorの円を描く。中はぬりつぶさない。

--- Surface#drawFilledCircle(x,y,r,color)
      SGEが必要 ロックが必要

      色がcolorの円を描き、中をぬりつぶす。

--- Surface#drawEllispe(x,y,rx,ry,color)
      SGEが必要 ロックが必要

      色がcolorの楕円を描く。中はぬりつぶさない。

--- Surface#drawFilledEllispe(x,y,rx,ry,color)
      SGEが必要 ロックが必要

      色がcolorの楕円を描く。中をぬりつぶす。。

--- Surface#rotateScaledSurface(angle,scale,bgcolor)
      SGEが必要 

      これは、selfをangle度傾け、scale倍した画像を持つSurfaceのインスタンスを
      生成するメソッドである。回転によって生じる隙間はbgcolorで埋められる。

--- Surface#rotateSurface(angle,bgcolor)
      rotateScaledSurfaceと同様。ただしscaleは1としたことになる。

--- Surface#mapRGB(r,g,b)
      selfのサーフィスのフォーマット(bppなど)に従ってr,g,bによって
      あらわされる色に対応したpixelの値を返す。

--- Surface#mapRGBA(r,g,b,a)
      ((<Surface#mapRGB>))と同様。ただしアルファ値も含めることができる。

--- Surface#getRGB(pixel)
      ((<Surface#mapRGB>))と逆の変換をする。返り値は [r,g,b]
      という内容の配列を返す。

--- Surface#getRGBA(pixel)
      ((<Surface#getRGB>))と同様。ただし返り値にアルファも含んでいる。
      返り値は[r,g,b,a]という内容の配列である。

--- Surface#bpp
      サーフィスのフォーマットの1ピクセルあたりのビット数を返す。

--- Surface#colorkey
      サーフィスに設定されたカラーキーの値を返す。

--- Surface#alpha
      サーフィスに設定されたアルファ値を返す。

=== Screen

ここにかきこまれた画像が画面に表示される。
このクラスはただ一つしか生成できないようになっていて、
SDL::setVideoModeによってのみ生成される。
実際には、このようなクラスは存在せず、Surfaceに以下の特異メソッドを追加した
オブジェクトである。

==== スーパークラス

((<Surface>))

==== クラスメソッド


==== メソッド

--- Screen#updateRect(x,y,w,h)
    この関数を呼び出すと、与えられた画面上の指定された長方形領域のリストが
    確実に更新される。
    x、y、w、hがすべて0の場合、全画面を更新する。画面がロックされてい
    るときにはこの関数を呼び出さないようにする。

--- Screen#flip
      ダブルバッファをサポートしているハードウェア上では、この関数は
      バッファの交換を要求して返る。ハードウェアによって次の垂直帰線期間まで
      待ってから、次のビデオサーフェスへのblitやロック操作の呼び出しが返る前
      にビデオバッファの交換が行われる。ダブルバッファをサポートしない
      ハードウェア上では、この呼び出しは((<Screen#updateRect>))(0, 0, 0, 0)
      としているのと同じである。このメソッドを使用してハードウェアによる画面の
      交換を行うためには、((<setVideoMode>))の引数でDOUBLEBUFフラグを設
      定しておくことが必要である。

=== PixelFormat

このクラスの機能はすべて((<Surface>))にうつされた。そちらを使用したほうが
よい。

SDL_PixelFormatのラッパークラス。
((<Surface#format>))で取得でき、((<Surface.new>))のformatとしてこの
オブジェクトを与えることができる。

SDLでは、16bit colorのsurfaceと32bitのsurfaceというように複数の画像の形式が
共存できる。よってその画像を操作するため、その形式に関する情報が
必要となる。PixelFormatのインスタンスが表しているものはそれである。

通常色を表すためにはr,g,bの値が使われるが、それをsurfaceの内部形式に従った
ひとつの値にするためにこれを用いる。逆の変換もできる。

このライブラリ内ではこれによって指定された値をもちいて色を指定する。

また、配列を用いて色を表すこともできる。これは r,g,b の値を
[r,g,b]で表すものである。

==== スーパークラス

Object

==== クラスメソッド

なし

==== メソッド

--- PixelFormat#mapRGB(r,g,b)
      selfのフォーマットに従ってr,g,bによってあらわされる色に対応した
      pixelの値を返す。

--- PixelFormat#mapRGBA(r,g,b,a)
      ((<PixelFormat#mapRGB>))と同様。ただしアルファ値も含めることができる。

--- PixelFormat#getRGB(pixel)
      ((<PixelFormat#mapRGB>))と逆の変換をする。返り値は [r,g,b]
      という内容の配列を返す。

--- PixelFormat#getRGBA(pixel)
      ((<PixelFormat#getRGB>))と同様。ただし返り値にアルファも含んでいる。
      返り値は[r,g,b,a]という内容の配列である。

--- PixelFormat#bpp
      フォーマットの1ピクセルあたりのビット数を返す。

--- PixelFormat#colorkey
      Not documented yet

--- PixelFormat#alpha
      Not documented yet

== Event関連

=== Event

イベントをとりあつかうクラス

==== スーパークラス

Object

==== クラスメソッド

--- Event.new

==== メソッド

--- Event#poll
      現在イベントキュー内で処理待ちしているイベントがないかをしらべ、
      ひとつでもあれば1を、ひとつもなければ0を返す。
      また、処理待ちをしているイベントがあれば、その情報をselfに格納する。

--- Event#wait
      次のイベントが来るまで待ち、来れば1を、またイベントを待つ間にエラー
      が生じた場合は0を返す。そしてその情報をselfに格納する。

--- Event#type
      格納されているイベントがいかなる種類のものであるかを返す。
      その種類は以下の定数で示される。
      
        Event::ACTIVEEVENT 
        Event::KEYDOWN
        Event::KEYUP
        Event::MOUSEMOTION
        Event::MOUSEBUTTONDOWN
        Event::MOUSEBUTTONUP
        Event::JOYAXISMOTION
        Event::JOYBALLMOTION
        Event::JOYHATMOTION
        Event::JOYBUTTONDOWN
        Event::JOYBUTTONUP
        Event::QUIT
        Event::SYSWMEVENT
        Event::VIDEORESIZE

--- Event#info
      イベントの情報を配列で返す。
      このメソッドは、これ以下のメソッドをすべて代用できる。

--- Event#keyPress?
      キーイベントでキーが押し下げられていればtrueを、いなければfalseを返す。

--- Event#keySym
      キーイベントで押し下げ/上げられたキーをを返す。

--- Event#keyMod
      キーイベントでの修飾キー(SHIFT,CTRLなど)の状態を返す。

--- Event#gain?
      ACTIVEEVENTイベントでウィンドウがフォーカスを得たならtrueを、
      失なったならばfalseを返す。

--- Event#appState
      ACTIVEEVENTイベントでのイベントの種類を返す。
      その内容は以下のいずれか。
        Event::APPMOUSEFOCUS
        Event::APPINPUTFOCUS
        Event::APPACTIVE

--- Event#mouseX
      マウスイベントでのマウスカーソルのX座標を返す。

--- Event#mouseY
      マウスイベントでのマウスカーソルのY座標を返す。      

--- Event#mouseXrel
      マウスイベントでのマウスカーソルのX座標の変化量を返す。

--- Event#mouseYrel
      マウスイベントでのマウスカーソルのX座標の変化量を返す。

--- Event#mouseButton
      マウスイベントでどのボタンのイベントであるかをかえす。
      それは以下の定数でしめされる。

        SDL::Mouse::BUTTON_LEFT  左ボタン
        SDL::Mouse::BUTTON_MIDDLE  中ボタン
        SDL::Mouse::BUTTON_RIGHT 右ボタン

--- Event#mousePress?
      MOUSEBUTTONDOWN,MOUSEBUTTONUPイベントにおいて、マウスボタンが
      押されたならtrue、離されたならfalseを返す。

=== Key

キーボードのキーに対応する定数を定義しているモジュール
キーボードのキーの状態を知るための関数もこの中にある。

==== モジュール関数

--- scan
      キーボードの状態をスキャンします

--- press?(key)
      これを呼び出す前にscanを呼びだしてください。
      scan で得た状態を得ます。trueで押している、falseで離しているです。

--- modState
      修飾キー(CTRL,ATL,など)の状態を返す。
      返り値は以下の定数でORをとったもので表される。
        Key::MOD_NONE
        Key::MOD_LSHIFT
        Key::MOD_RSHIFT
        Key::MOD_LCTRL
        Key::MOD_RCTRL
        Key::MOD_LALT
        Key::MOD_RALT
        Key::MOD_LMETA
        Key::MOD_RMETA
        Key::MOD_NUM
        Key::MOD_CAPS
        Key::MOD_MODE
        Key::MOD_RESERVED
        Key::MOD_CTRL = Key::MOD_LCTRL|Key::MOD_RCTRL
        Key::MOD_SHIFT = Key::MOD_LSHIFT|Key::MOD_RSHIFT
        Key::MOD_ALT = Key::MOD_LALT|Key::MOD_RALT
        Key::MOD_META = Key::MOD_LMETA|Key::MOD_RMETA

--- enableKeyRepeat(delay,interval)
      キーリピートの設定を変える。
      

--- disableKeyRepeat
      キーリピートを無効にする。
    
== Mouse

マウス関連の定数と関数を定義しているモジュール

=== モジュール関数

--- state
      マウスの状態を配列の形で返します。
      配列の内容は
        [ x , y , pressLButton? , pressMButton? , pressRButton? ]
      となっています。

--- warp(x,y)
      マウスカーソルの位置を設定する（マウス移動のイベントを生成する)。

--- show
      マウスカーソルを表示する。

--- hide
      マウスカーソルを消す。

--- setCursor(bitmap,white,black,transparent,inverted,hot_x=0,hot_y=0)
      マウスカーソルを変える。
      bitmapとして((<Surface>))のインスタンスを与える。
      カーソルの色は白黒で生成される。
      white,black,transparent,invertedはそのサーフィス内で、どの
      ピクセルがカーソルの白、黒、透明、反転にあたるかを示す。

== audio関連

=== Mixer

音をだしたりするのに使うモジュール
ボリュームは0から128が有効である。
このモジュールの機能を使うためには、SDL_mixerライブラリが必要である。
このモジュール内の機能を使うためには、initでINIT_AUDIOを有効にしな
ければならない。

==== モジュール関数

--- open(frequency=Mixer::DEFAULT_FREQUENCY,format=Mixer::DEFAULT_FORMAT,cannels=Mixer::DEFAULT_CHANNELS,chunksize=4096)
      このモジュールの機能の初期化関数。
      frequencyは周波数、formatはサウンドの形式、
      channelsは1でモノラル、2でステレオ
      chunksizeはバッファの大きさ、をそれぞれ指定する。
      chunksizeは2の階乗を使うようにする。
      ここでいうchannelsとplayChannelなどでのchannelは別物である。

--- spec
      初期化したオーディオの性能を配列で返す。
      その内容は、
        [ rate,format,channels ]

--- playChannel(channel,wave,loop)
      指定したchannelでwaveを演奏する。
      channelに-1を指定すると、あいているchannelが適当にえらばれる。
      loops指定した回数繰り返す。
      loopsが-1のときは際限なくくりかえす。
      loopsが0のときは一度のみ演奏する。

      どのチャンネルを演奏に利用したかを返す。

--- play?(channel)
      指定したchannelが現在演奏していればtrueを、していなければ
      falseを返す。

--- setVolume(channel,volume)
      指定したchannelのボリュームを設定する。
      channel=-1を指定するとすべてのchannelに対しボリュームを指定する。

      volume=-1とすると、現在のボリュームが返る。

--- halt(channel)
      指定したchannelの演奏を止める。

--- pause(chennel)
      指定したchannelの演奏を一時停止する。

--- resume(channel)
      指定した一時停止しているchannelの演奏を再開する。

--- pause?(channel)
      指定したchannelが一時停止していればtrue、していなければfalseを
      返す。

--- playMusic(music,loops)
      musicで指定した音楽を演奏する。
      loopsは((<playChannel>))と同じ。

--- fadeInMusic(music,loops,ms)
      musicで指定した音楽をフェードインして演奏する。
      loopsは((<playChannel>))と同じ。
      フェードインはmsは指定したミリ秒だけかける

--- setVolumeMusic(volume)
      音楽のボリュームを指定する。

--- haltMusic
      音楽を止める。

--- fadeOutMusic(ms)
      音楽を指定したミリ秒かけてフェードアウトする。

--- pauseMusic
      音楽を一時停止する。
      
--- resumeMusic
      一時停止している音楽の再生を再開する。

--- rewindMusic
      音楽の再生位置を一番最初にする。

--- pauseMusic?
      音楽が一時停止していればtrue、いなければfalseを返す。

--- playMusic?
      音楽が演奏されていればtrue、していなければfalseを返す。

=== Mixer::Wave

Waveをあらわすクラス

==== スーパークラス

Object

==== クラスメソッド

--- Mixer::Wave.load(filename)
      waveファイルをロードし、それに対応するMixer::Waveクラスの
      インスタンスを返す。

==== メソッド

--- Mixer::Wave#setVolume(volume)
      selfのボリュームを返す。

=== Mixer::Music

音楽(.mod .s3m .it .xm .mid .mp3)を表す。
ただしmidiファイルやMP3を演奏するためにはそのための設定が必要。

==== スーパークラス

Object

==== クラスメソッド

--- Mixer::Music.load(filename)
      音楽(.mod .s3m .it .xm .mid .mp3)をファイルからロードし、
      そのデータに対応するMixer::Musicクラスのインスタンスを返す。

== Window Manager 関連

=== WM

Window関連の処理をまとめたモジュール

==== モジュール関数

--- caption
      captionの文字列を配列で返してくる。
      内容は、
        [ ウィンドウのタイトル , アイコンのタイトル ]

--- setCaption(title,icon)
      上記の内容を変更する。

--- icon=(iconImage)
      ウィンドウのアイコンの絵を指定する。
      setVideoModeの呼びだし前に呼びださなければならない。
      
--- iconify
      ウィンドウのアイコン化、最小化をする。
      成功すれば、Event::APPACTIVEイベントが生じる。

== CDROM関係

=== CD

CDROMドライブを表すクラス

CD#numTrack等の情報はCD#statusを呼びだすことによって更新される。

ここではCDの位置、演奏時間の長さを表すためにフレームという単位を用いる。
1frame=2Kであり、
通常の音楽CDにおいて75フレーム=1秒である。

=== スーパークラス

Object

=== クラスメソッド

--- CD.numDrive
      いくつのCDROMドライブが使用できるかを返す。

--- CD.name(drive)
      指定したドライブの名前を文字列で返す。
      ドライブの指定は0からCD.numDrive-1の整数で指定する。

--- CD.open(drive)
      指定したドライブを開く。
      成功すればCDのインスタンスが返る。
      ドライブの指定は0からCD.numDrive-1の整数で指定する。
      0ドライブがデフォルトのドライブである。

=== メソッド

--- CD#status
      currentTrack,currentFrame,numTracks,trackType,trackLenght
      の情報を更新する。
      現在のCDの状態を返す。その内容は以下のとおり。
        TRAYEMPTY
        STOPPED
        PLAYING
        PAUSED
        ERROR

--- CD#play(start,length)
      CDをstartフレームからlengthフレームの間演奏する。

--- CD#playTrack(start_track,start_frame,ntracks,nframes)
      CDをstart_trackのstart_frameから、ntracks先のトラックのnframeのところ
      まで演奏する。
      なお、このライブラリ内ではトラックのインデックスは0からはじまる。
      このメソッドは((<CD#status>))を呼びだしたあとに呼びださなければならない。

        例
        cd.playTrack(0,0,1,0) # 最初のトラックを演奏する。
        cd.playTrack(1,0,0,SDL::CD::FPS*15) # 2トラック目を最初から15秒演奏する。

--- CD#pause
      CDの演奏を一時停止する。

--- CD#resume
      CDの演奏を再開する。

--- CD#stop
      CDの演奏を止める。

--- CD#eject
      CDをイジェクトする。

--- CD#numTracks
      CDのトラック数を返す。

--- CD#currentTrack
      現在演奏しているトラックを返す。

--- CD#currentFrame
      現在演奏している位置をフレーム数で返す。
      このあたいは現在のトラックの最初からの値である。

--- CD#trackType(track)
      指定したトラックがなんであるかを返す。その値は以下のとおり。
        AUDIO_TRACK
        DATA_TRACK

--- CD#trackLength(track)
      指定したトラックの長さをフレーム数で返す。

== ジョイスティック関連

=== Joystick

一つのジョイスティックを表すクラス

=== スーパークラス

Object

=== クラスメソッド

--- Joystick.pall
      イベントのpollingに合わせて、Joystick.updateAllを呼びだすかどうか
      を得る。
      
--- Joystick.pall=(polling)
      イベントのpollingに合わせて、Joystick.updateAllを呼びだすかどうか
      を設定する。

--- Joystick.num
      利用可能なジョイスティックの数を返す。

--- Joystick.name(index)
      指定したジョイスティックの名前を文字列で返す。
      0からJoystick.num-1の整数で指定する。

--- Joystick.open(index)
      指定したジョイスティックを開く。
      成功するとJoystickのインスタンスが返ってくる。

--- Joystick.open?(index)
      指定したジョイスティックがすでに開かれているかを返す。

--- Joystick.updateAll
      全てのジョイスティックの情報を更新する。
      Joystick#button/ball/hat/axisの情報はこれで更新される。

=== メソッド

--- Joystick#index
      ジョイスティックに対応する整数を返す。

--- Joystick#numAxes
      いわゆるアナログ入力装置の数を返す。
      ただし、2軸のアナログティックは2個の装置があると数えられる。

--- Joystick#numBalls
      トラックボールの数を返す。

--- Joystick#numButtons
      ボタンの数を返す。

--- Joystick#axis(axis_index)
      アナログ入力装置の入力状態を返す。
      その値は-32768から32768である。
      通常0にはx軸、1にはy軸があてられる。

--- Joystick#hat(hat_index)
      いわゆる十字キーの入力状態を返す。その内容は以下のとおり。
        Joystick::HAT_CENTERED
        Joystick::HAT_UP
        Joystick::HAT_RIGHT
        Joystick::HAT_DOWN
        Joystick::HAT_LEFT
        Joystick::HAT_RIGHTUP
        Joystick::HAT_RIGHTDOWN
        Joystick::HAT_LEFTUP
        Joystick::HAT_LEFTDOWN

--- Joystick#button(button_index)
      ボタンの押下情報を返す。
      押していればtrue、いなければfalseを返す。

--- Joystick#ball(ball_index)
      トラックボールの入力情報を返す。
      その内容は最後にこのメソッドを呼び出してからX軸、Y軸にどれだけ
      動かしたかを配列 [dx,dy] で返す。

== フォント関係

=== TTF

ＴｒｕｅＴｙｐｅフォントを表すクラス

==== スーパークラス

Object

==== クラスメソッド

--- TTF.init
      TrueTypeFontを使うためには最初にこの関数を呼ばなければならない。

--- TTF.open(filename,size)
      TrueTypeFontをファイルから読み出し、そのサイズをsizeで指定する。
      
==== メソッド

--- TTF#style
      フォントのスタイルを返す。

--- TTF#style=(style)
      フォントのスタイルを設定する。指定できるのは以下の定数でORをとったもの。
        TTF::STYLE_NORMAL
        TTF::STYLE_BOLD
        TTF::STYLE_ITALIC
        TTF::STYLE_UNDERLINE

--- TTF#textSize(text)
      textを描画したときの縦、横の必要な大きさを配列で返す

--- TTF#drawSolidUTF8(dest,text,x,y,r,g,b)
      selfのフォント設定でdest(Surfaceのインスタンス)にString textを
      destの位置(x,y)の所に書きこむ。色はr,g,bで決められる。
      透明色(ColorKey)は有効である。textはUTF-8を使う。

--- TTF#drawBlendedUTF8(dest,text,x,y,r,g,b)
      drawSolidUTF8と同様。drawSolidUTF8よりも高品質な描画ができる。

== 時刻処理

=== モジュール関数

--- getTicks
      SDL.initが呼ばれてからの時間をミリ秒で返す。
      49日ほどスクリプトを走らせつづけると0にもどる。

--- delay(ms)
      ミリ秒で指定された時間だけ待つ。
      OSのスケジューリングのため指定した時間よりも長く待つ可能性がある。

=end
