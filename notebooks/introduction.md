# Rakuten Multi-modal Colour Extraction

- Date: 18 Février 2021
- Auteur: Itokiana Rafidinarivo

<div style="align:center">
    <img src="https://challengedata.ens.fr/logo/public/RIT_logo_big_YnFAcFo.jpg" style="width:35%">
<div>
    
## Contexte
- **Rakuten** a été créée en 1997 au Japon et à l'origine du concept de *"marketplace"* et est ainsi devenue une des plateformes les plus larges de **e-commerce**.
- Un challenge a été mis par Rakuten Institute of Technology localisé à Paris afin de prédire les couleurs d'un produit à partir de son image, son titre et sa description.
    
# Table des matières
- Structure du projet/fichiers
- Aperçu des données 
    
# Structure du projet/fichiers


```python
!tree -L 1
```

    [01;34m.[00m
    ├── X_test.csv
    ├── X_train.csv
    ├── [01;34mimages[00m
    ├── introduction.ipynb
    ├── metrics.py
    ├── sample_submission.csv
    └── y_train.csv
    
    1 directory, 6 files



```python
!echo " Nombre d'images: $(ls \images | wc -l)"
```

     Nombre d'images:   249470


- X_train.csv et y_train.csv sont les fichiers d'entraînement.
- X_test.csv est le fichier sur lequel il faudra faire les prédictions.
- metrics.py est un script Python qui contient le metric spécifique au problème ainsi qu'un
exemple de calcul de score avec lecture des fichiers.
- images est la directory contenant les images des produits.

# Aperçu des données


```python
import pandas as pd
import numpy as np
```


```python
X_train    = pd.read_csv('X_train.csv', index_col=0)
y_train    = pd.read_csv('y_train.csv', index_col=0)
X_test     = pd.read_csv('X_test.csv', index_col=0)
sample_sub = pd.read_csv('sample_submission.csv', index_col=0)
```

## X_train.csv


```python
X_train.head()
```




<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>image_file_name</th>
      <th>item_name</th>
      <th>item_caption</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>0</th>
      <td>278003_10389968_1.jpg</td>
      <td>三協アルミ M.シェード2 梁置きタイプ 片側支持 5818 H30 ポリカーボネート屋根　...</td>
      <td>商品番号19235601メーカー三協アルミサイズ幅 1931.0mm × 奥行き 5853....</td>
    </tr>
    <tr>
      <th>1</th>
      <td>220810_10010506_1.jpg</td>
      <td>【40%OFF SALE/セール】30代〜40代 ファッション コーディネート 太サッシュ ...</td>
      <td>太サッシュベルトで存在感アップ 柔軟性に優れた馬革を使用 幅が太めで存在感◎ キレイな形が出...</td>
    </tr>
    <tr>
      <th>2</th>
      <td>207456_10045549_1.jpg</td>
      <td>下駄 桐 日本製 女性用 TONE 鼻緒巾が広め 黒塗り台 適合足サイズ 23〜24.5cm...</td>
      <td>項目 桐の下駄 ※特別価格にて浴衣、半幅帯（浴衣帯）、巾着等も同時出品中です！ サイズ 下駄...</td>
    </tr>
    <tr>
      <th>3</th>
      <td>346541_10000214_1.jpg</td>
      <td>＼期間限定【1000円OFF】クーポン 発行中／ シューズボックス 幅60 奥行33 15足...</td>
      <td>■商品説明 ルーバーシューズボックス60幅のシングルタイプが登場。お部屋に合わせて色、サイズ...</td>
    </tr>
    <tr>
      <th>4</th>
      <td>240426_10024071_1.jpg</td>
      <td>ポスト 郵便ポスト 郵便受け 集合住宅用ポスト 可変式プッシュ錠集合郵便受箱 PKS-M15...</td>
      <td>集合住宅用ポスト 可変式プッシュ錠集合郵便受箱 PKS-M15-3 1列3段 暗証番号を自由...</td>
    </tr>
  </tbody>
</table>
</div>



## y_train.csv


```python
y_train.head()
```




<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>color_tags</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>0</th>
      <td>['Silver', 'Grey', 'Black']</td>
    </tr>
    <tr>
      <th>1</th>
      <td>['Brown', 'Black']</td>
    </tr>
    <tr>
      <th>2</th>
      <td>['White', 'Black']</td>
    </tr>
    <tr>
      <th>3</th>
      <td>['Beige', 'Brown', 'Black']</td>
    </tr>
    <tr>
      <th>4</th>
      <td>['Silver']</td>
    </tr>
  </tbody>
</table>
</div>



## X_test.csv


```python
X_test.head()
```




<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>image_file_name</th>
      <th>item_name</th>
      <th>item_caption</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>0</th>
      <td>295152_23116034_1.jpg</td>
      <td>KYV39 miraie f ミライエ フォルテ kyv39 au エーユー スマホ ケース...</td>
      <td>注意事項 素材ケースの入荷時期により、商品写真とは一部カメラ・ボタン穴などの形状が異なるケー...</td>
    </tr>
    <tr>
      <th>1</th>
      <td>295152_26516148_1.jpg</td>
      <td>SO-04K Xperia XZ2 Premium エクスペリア エックスゼットツー プレミ...</td>
      <td>注意事項素材ケースの入荷時期により、商品写真とは一部カメラ・ボタン穴などの形状が異なるケース...</td>
    </tr>
    <tr>
      <th>2</th>
      <td>295152_24772914_1.jpg</td>
      <td>MO-01K MONO モノ mo01k docomo ドコモ 手帳型 スマホ カバー カバ...</td>
      <td>商品特徴 ・シームレスな全面デザイン（内側は落ち着いたオフホワイト） ・外側は高品質の人工革...</td>
    </tr>
    <tr>
      <th>3</th>
      <td>284888_11730362_1.jpg</td>
      <td>Xperia XZ 手帳型ケース ビーチ ハワイ エクスペリア ケース カバー ケイオー ブ...</td>
      <td>対応機種Xperia XZ ( エクスペリア ) ソニー対応型番XperiaXZキャリアDo...</td>
    </tr>
    <tr>
      <th>4</th>
      <td>245178_17489104_1.jpg</td>
      <td>【中古】コムサデモード COMME CA DU MODE スカート ボムトス ロング丈 斜め...</td>
      <td>【中古】コムサデモード COMME CA DU MODE スカート ボムトス ロング丈 斜め...</td>
    </tr>
  </tbody>
</table>
</div>



## sample_submission.csv


```python
sample_sub.head()
```




<div>
<style scoped>
    .dataframe tbody tr th:only-of-type {
        vertical-align: middle;
    }

    .dataframe tbody tr th {
        vertical-align: top;
    }

    .dataframe thead th {
        text-align: right;
    }
</style>
<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>color_tags</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>0</th>
      <td>['Transparent']</td>
    </tr>
    <tr>
      <th>1</th>
      <td>['Silver']</td>
    </tr>
    <tr>
      <th>2</th>
      <td>['Brown', 'Khaki']</td>
    </tr>
    <tr>
      <th>3</th>
      <td>['Burgundy', 'Pink']</td>
    </tr>
    <tr>
      <th>4</th>
      <td>['Green', 'Pink']</td>
    </tr>
  </tbody>
</table>
</div>


