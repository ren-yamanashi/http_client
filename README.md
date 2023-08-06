# Usage

1. `main.c` ファイルで、以下のようにリクエスト情報を設定します。

```c
#include "constance.h"
#include "client.h"

int main()
{
    HttpRequest request;
    /**
     * HttpRequest構造体は、左からそれぞれ以下のような意味を持ちます
     * - HTTPリクエストメソッド
     * - リクエストターゲット リクエスト可能なパスを指定します ex: `/user/23`
     * - オリジン リクエストの受付可能なサーバーのオリジンを指定します
     * - リクエストボディ
     * - コンテンツタイプ
     */
    createRequest(&request, HTTP_METHOD_GET, "/", "http://localhost:8080", NULL, NULL);
    // POSTメソッドの場合は以下のように設定できます
    // createRequest(&request, HTTP_METHOD_POST, "/sample", "http://localhost:8080", "{'name': 'johnDoe'}", "application/json");
    httpRequestWithConnection(&request);
}
```

2. 以下のコマンドで、リクエストを送信します
```bash
make
```

