🏴󠁧󠁢󠁷󠁬󠁳󠁿 Algoritmo de Welsh-Powell para Coloração de Grafos
💡 Implementação Simples do Algoritmo
O algoritmo de Welsh-Powell é uma heurística¹ gulosa² simples e eficiente para a coloração de grafos.

Ele consiste, basicamente, nos seguintes passos:

Ordenar todos os vértices pelo seu grau (número de conexões que o vértice faz), geralmente em ordem decrescente.

Partindo desse conjunto de vértices ordenado, colorir um a um, utilizando a primeira cor disponível que não seja usada por nenhum de seus vizinhos.

Embora o Welsh-Powell não garanta a coloração ótima (o menor número de cores) em todos os casos, ele tende a produzir bons resultados em um tempo razoável.

📝 Conceitos Chave
1. Heurística

No contexto da computação, a heurística refere-se a uma abordagem ou método de resolução de problemas que utiliza regras práticas ou "atalhos" para encontrar soluções viáveis e suficientemente boas em um tempo razoável, em vez de garantir a solução ótima ou perfeita.

2. Algoritmo Guloso (Greedy Algorithm)

Um algoritmo guloso é uma estratégia de resolução de problemas que, a cada passo, faz a escolha que parece ser a melhor localmente, sem considerar as consequências futuras.

Essa abordagem toma uma decisão momentânea e não retrocede (não volta atrás nas decisões).

Espera-se que essa sequência de escolhas locais leve a uma solução global ótima.

Por não voltar atrás nas decisões, algoritmos gulosos são geralmente rápidos e eficientes, embora nem sempre garantam a melhor solução final para todos os problemas.


