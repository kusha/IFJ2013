IFJ2013
=======

Projekt interpretu jazyka IFJ2013

---

Pokud jste videte tento readme file, tak uz asi mate account na github'u =) a odeslali mne svuj username.

###Jak zacat pracovat?

#### 1. musime vygenerovat klic pro autorizace na github

Pripojte se k merlinu, pak:

        cd ~/.ssh
        la -al

pokud tam jsou soubory:

        id_rsa.pub, id_dsa.pub

tak uz mate vygenerovany public key =) a muzete jit na krok 3, pokud ne - pristi krok je druhy

#### 2. generace public key

        ssh-keygen -t rsa -C "<VAS LOGIN>@stud.fit.vutbr.cz"

zkontrolujte abych ten prikaz vypsal:

        Generating public/private rsa key pair.
        Enter file in which to save the key (/homes/eva/<ASI ZACATEK LOGINU>/<VAS LOGIN>/.ssh/id_rsa):<STISKNETE ENTER>
        Enter passphrase (empty for no passphrase):<NAPISTE PASS (ABYCH NEZAPOMENOUT POUZIVEJTE PASS FITU)>
        Enter same passphrase again:<NAPISTE PASS (ABYCH NEZAPOMENOUT POUZIVEJTE PASS FITU)>
        Your identification has been saved in /homes/eva/<ASI ZACATEK LOGINU>/<VAS LOGIN>/.ssh/id_rsa.
        Your public key has been saved in /homes/eva/<ASI ZACATEK LOGINU>/<VAS LOGIN>/.ssh/id_rsa.pub.
        The key fingerprint is:
        01:0f:f4:3b:ca:85:d6:17:a1:7d:f0:68:9d:f0:a2:db  <VAS LOGIN>@stud.fit.vutbr.cz
        The key's randomart image is:
        <ZAJMAVY OBRAZEK>

jste mate vygenerovany klic =)

#### 3. musime zapsat ten klic ho do github'u

*nikdo nevyzvedne vas pas - odesilame jenom piblic key*

        cat ~/.ssh/id_rsa.pub

vykopirujte ho, pak otevrete stranku https://github.com/settings/ssh, stisknete Add SSH key

        Title: GitHub for Merlin (nebo neco jineho - co chcete )
        Key: <TENTO id_rsa.pub>

naposled - Add KEY 

#### 3.5 ZKONTROLUJTE USPESNOST PREDCHOZICH KROKU

z merlinu:

        ssh -T git@github.com

toto poprosi napsat vas passphrase a pak, pokud vsechno je v poradku, vytiskne vas username =)

#### 4. spojeni s repozitarem

otevrte nejaky adresar na merlinu a vypln'te:

        git clone git@github.com:kusha/IFJ2013.git

to udela novy adresar s obsahem repositarze

#### NAVODY NA POUZITI GIT:

Nejjednodussi pouziti:
udelali jste zmeny (napiklad napsali novou funkce)

        git add *                       //najde vsechny zmeny ve vsech souborech adresaru
        git commit -m "Nova funkce"     //zapise vsechny zmeny v lokalni file .git
        git push origin master          //odesle zmeny na server

[ENG Kratka prezentace zakladnych prikazu](http://rogerdudler.github.io/git-guide/)

[ENG Interaktivni studium github za 15 minut](http://try.github.io/)

[CZ Extra advanced book](http://git-scm.com/book/cs)

[RU Extra advanced book](http://git-scm.com/book/ru)
