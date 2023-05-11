
#include <iostream>
#include <algorithm>

using namespace std;

typedef int KuyrukVeriTipi;

struct Dugum {
	int islemSuresi;
	int toplamBeklemeSuresi;
	int siraNo;

	Dugum* sonraki;
};

struct Kuyruk {
	Dugum* bas;
	Dugum* son;
	Dugum* gecici;
	int siraSayisi = 0;
	void olustur();
	void kapat();
	void ekle(KuyrukVeriTipi);
	void cikar();
	bool bosmu();
	KuyrukVeriTipi ortalama();
	void yazdir();
	int siradakiKisi(KuyrukVeriTipi,int&);
};

struct DiziKuyruk
{
	int* dizi = NULL;
	int diziBas = 0;
	int diziSon = 0;
	int boyut = 2;
	void diziEkle(KuyrukVeriTipi);
	void diziToparla();
	int diziCikar();
	void diziSirala(KuyrukVeriTipi);
};

int DiziKuyruk::diziCikar() {
	if (diziSon == diziBas)
	{
		cout << "sira bos." << endl;
		return -1;
	}
	if (diziSon - diziBas <= boyut / 4)
	{
		int* dizi2 = (int*)malloc(sizeof(int) * boyut / 2);
		for (int i = 0; i < boyut / 2; i++)
		{
			dizi2[i] = dizi[i];
		}
		free(dizi);
		boyut /= 2;
		dizi = dizi2;
	}
	return dizi[diziBas++];
}

void DiziKuyruk::diziToparla() {
	if (diziBas == 0)
	{
		return;
	}
	for (int i = 0; i < boyut; i++)
	{
		dizi[i] = dizi[i + diziBas];
	}
	diziSon -= diziBas;
	diziBas = 0;
}

void DiziKuyruk::diziEkle(KuyrukVeriTipi yeniDizi) {
	if (dizi == NULL)
	{
		dizi = (int*)malloc(sizeof(int) * 2);
	}
	if (diziSon >= boyut) {
		boyut *= 2;
		int* dizi2 = (int*)malloc(sizeof(int) * boyut);
		for (int i = 0; i < boyut / 2; i++)
		{
			dizi2[i] = dizi[i];
		}
		free(dizi);
		dizi = dizi2;
	}
	dizi[diziSon++] = yeniDizi;
}

void Kuyruk::olustur() {
	bas = NULL;
	son = NULL;
}

void Kuyruk::kapat() {
	Dugum* p;
	while (bas) {
		p = bas;
		bas = bas->sonraki;
		delete p;
	}
}

void Kuyruk::ekle(KuyrukVeriTipi yeni) {
	Dugum* yeniDugum = new Dugum;
	siraSayisi++;
	yeniDugum->islemSuresi = yeni;
	yeniDugum->siraNo = siraSayisi;
	yeniDugum->toplamBeklemeSuresi = yeni;
	yeniDugum->sonraki = NULL;
	if (bas == NULL) {
		bas = yeniDugum;
		son = bas;
	}
	else {
		Dugum* tara;
		tara = son;
		yeniDugum->toplamBeklemeSuresi += tara->toplamBeklemeSuresi;
		son->sonraki = yeniDugum;
		son = yeniDugum;
	}
}

void Kuyruk::cikar() {
	Dugum* ustDugum;
	if (bosmu())
	{
		cout << "Daha fazla oge cikarilamaz icerisi bos" << endl;
	}
	else
	{
		ustDugum = bas;
		siraSayisi--;
		bas = bas->sonraki;
		delete ustDugum;
	}
}

bool Kuyruk::bosmu() {
	return bas == NULL;
}

void Kuyruk::yazdir() {
	if (bosmu()) {
		cout << "Bos..." << endl;
		return;
	}
	Dugum* ustDugum;
	ustDugum = bas;
	for (int i = 0; i <= siraSayisi; i++)
	{
		if (ustDugum != NULL)
		{
			cout << ustDugum->siraNo << ".siradaki kisinin islem suresi : " << ustDugum->islemSuresi << " -- toplam bekleme suresi : " << ustDugum->toplamBeklemeSuresi << endl;
			ustDugum = ustDugum->sonraki;
		}
	}
	cout << "----------------------------------" << endl;
}

KuyrukVeriTipi Kuyruk::ortalama() {
	int ort = 0;
	int toplamKisiSayisi = siraSayisi;
	Dugum* tara = bas;
	if (bosmu())
	{
		cout << "Sira bos!!!";
		return 0;
	}
	else
	{
		while (tara)
		{
			ort += tara->toplamBeklemeSuresi;
			tara = tara->sonraki;
		}
		return ort / toplamKisiSayisi;
	}
}

void DiziKuyruk::diziSirala(KuyrukVeriTipi kisiSayisi) {
	int tmp;
	int min;

	for (int i = 0; i < kisiSayisi - 1; i++)
	{
		min = i;

		for (int j = i; j < kisiSayisi; j++)
		{
			if (dizi[j] < dizi[min])
			{
				min = j;
			}
		}
		tmp = dizi[i];
		dizi[i] = dizi[min];
		dizi[min] = tmp;
	}
}

int Kuyruk::siradakiKisi(KuyrukVeriTipi sure, int& fark) {
	if (bosmu())
	{
		cout << "sira bos." << endl;
		return 0;
	}

	Dugum* tara;
	Dugum* temp;
	int tempSayi = 0;
	tara = gecici;

	if (tara != NULL && sure == tara->islemSuresi)
	{
		tempSayi = tara->siraNo;
		fark = tara->toplamBeklemeSuresi;
		temp = tara;
		if (tara->sonraki == NULL)
		{
			delete gecici;
			return tempSayi;
		}
		tara = tara->sonraki;
		gecici = tara;
		delete temp;
		return tempSayi;
	}
	while (tara->sonraki != NULL && sure != tara->sonraki->islemSuresi)
	{
		tara = tara->sonraki;
	}
	if (tara->sonraki != NULL)
	{
		tempSayi = tara->sonraki->siraNo;
		fark = tara->sonraki->toplamBeklemeSuresi;
		temp = tara->sonraki;
		tara->sonraki = temp->sonraki;
		delete temp;
		return tempSayi;
	}
	return 0;

}

int main() {
	Kuyruk* k = new Kuyruk();
	k->olustur();

	DiziKuyruk* dk = new DiziKuyruk();

	int kisiSayisi;
	cout << "Kuyrukta ki kisi sayisi : ";
	cin >> kisiSayisi;

	srand(time(0));

	int j = 0;

	for (int i = 0; i < kisiSayisi; i++) {
		j = rand() % (300 - 30 + 1) + 30;
		k->ekle(j);
		dk->diziEkle(j);
	}
	k->gecici = k->bas;
	k->yazdir();

	cout << "Sirayla ortalama bekleme suresi : " << k->ortalama() << endl;
	cout << "----------------------------------" << endl;

	dk->diziToparla();
	dk->diziSirala(kisiSayisi);

	cout << "Sure sayisi kucukten buyuge dogru siralanmis sira : " << endl;

	int diziToplamSure = 0;
	int ortalamaSuresi = 0;
	int fark = 0;

	for (int i = 0; i < kisiSayisi; i++)
	{
		diziToplamSure += dk->dizi[i];
		ortalamaSuresi += diziToplamSure;

		cout << k->siradakiKisi(dk->dizi[i],fark) << ".kisinin bekleme suresi : " << dk->dizi[i] << " -- Toplam suresi : " << diziToplamSure << endl;
		cout << "FIFO'ya gore arasındaki fark : " << (fark - diziToplamSure) << endl;
		cout << "----------------------------------" << endl;
	}
	cout << "----------------------------------" << endl;
	cout << "Kucukten buyuge siralamada ortalama sure : " << ortalamaSuresi / kisiSayisi << endl;
}
