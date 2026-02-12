//変更禁止
#include "Line.h"
#include "AIP.h"
#include "Input.h"


void LINE::get_message(byte* message) {
        //メッセージ処理

        //初期化
        //グループ分け

    if (pack_NUM == 0) { //ラインない
        line_x = 999;
        line_y = 999;
    } else { //ラインある
        //座標計算（移動平均）
        //俯瞰座標に変換
        //ゼロ除算防止
        //初回検知か確認（バグ防止）
        //反転判定（半分を超えても逃げる方向を維持するため）
        //逃げる方向更新（反転ステータスに応じて更新　解説みてね）
    
        
        //ラインセンサー処理だけ非公開なので簡単解説：
        //ライン処理は完全にベクトルを使用（AIPフォルダ内のMyVector.cppを参照）
        //方向反転に a = -a　を使えるから楽
        //ゼロ除算防止とか言ってるけど(0, 0)になったら前回の値を使うだけ　これで問題ないと勝手に思ってる
        //俯瞰座標（GAM.cppを参照 birdは鳥って意味だから俯瞰ってこと）を使用してIN判定　初回検知と最終検知の最小角度差が規定値超えたら～って
        //INを使うかどうかは環境次第だけど一応実装してる
        //メッセージ処理はラインマイコン側に全部（詰め込み、展開）あるから探してみてね
        //P.S. 全国で声かけてくれたら原文見せます - Fox
    }

    //IN判定更新（ペナルティーエリア内に入った場合は立てる）
    //条件付きリセット（INが立っていたら一部そのまま）
    //ライン種類更新
    //更新
        
}

int LINE::get_azimuth() {
    if (line_x == 0 && line_y == 0) {
        return 0;
    }
    return round(myvector.get_azimuth(line_x, line_y));
}

int LINE::get_magnitude() {
    int magnitude = 999;
    if (line_type != 0) {
        magnitude = myvector.get_magnitude(line_x, line_y);
    }
    return magnitude;
}

int LINE::get_eazimuth() {
    if (escape_x == 0 && escape_y == 0) {
        return 0;
    }
    return round(myvector.get_azimuth(escape_x, escape_y));
}

int LINE::get_x() {
    return line_x;
}

int LINE::get_y() {
    return line_y;
}

bool LINE::get_stat(byte lineNUM) {
    return line_stat[lineNUM];
}

int LINE::get_type() {
    return line_type;
}

int LINE::get_pack(byte packNUM) {
    return myvector.get_azimuth(pack_x[packNUM], pack_y[packNUM]);
}

bool LINE::get_trip() {
    return trip;
}

bool LINE::get_in() {
    return in;
}
