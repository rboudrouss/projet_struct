Réponses aux questions
======================

1. 
    1. La complexité de la fonction **`is_prime_naif`** est $\mathcal{O}(n)$.

    2. $6163$ est le plus petit nombre premier calculé en 2 milisecondes ou plus.

    3. **`modpow_naif``** a pour complexité $\mathcal{O}(n)$.

    4. Performance des fonctions **`modpow`**:

    ![Graphe de Modpow](img/modpow.png){width=50%}
    ![Graphe de Modpow_naive](img/modpow_naive.png){width=50%}

    Dans les graphes ci-dessous on voit bien que la fonction **`modpow`** est linéaire (du moins jusqu'à 10000) alors que le temps d'execution de **`modpow_naive`** est linéaire et augmente avec n.
    On peut donc conclure que **`modpow`** est beaucoup plus rapide surtout pour les grands nombres.
    
7\. 
    8. À partir de $d=4$, la fonction **`compute_proof_of_work`** prend plus d'une seconde sur ma machine. Le temps de calcule monte très vite cependant.

8\. 
    8. la fonction **`fusion_chaines`** est en $\mathcal{O}(n)$. On aurait pu introduire des listes doublement chaine pour que ça soit en $\mathcal{O}(1)$.
