import networkx as nx
import matplotlib.pyplot as plt
import sys

def desenhar_grafo():
    try:
        # Lê o arquivo gerado pelo C
        with open("dados_grafo.txt", "r") as f:
            linhas = f.readlines()
    except FileNotFoundError:
        print("Erro: O arquivo 'dados_grafo.txt' não foi encontrado.")
        return

    G = nx.Graph()
    node_colors = {}

    modo = "NODES"

    mapa_cores = {
        'A': '#FF3333',
        'B': '#00FF00',
        'C': '#0033FF',
        'D': '#F7CAC9',
        'E': '#92A8D1',
        'F': '#955251',
        '0': '#DDDDDD'
    }

    for linha in linhas:
        linha = linha.strip()
        if linha == "ARESTAS":
            modo = "EDGES"
            continue

        partes = linha.split()

        if modo == "NODES":
            if len(partes) == 2:
                no_id = int(partes[0])
                cor_char = partes[1]
                G.add_node(no_id)
                node_colors[no_id] = mapa_cores.get(cor_char, '#999999')

        elif modo == "EDGES":
            if len(partes) == 2:
                u = int(partes[0])
                v = int(partes[1])
                G.add_edge(u, v)

    plt.figure(figsize=(10, 8))
    pos = nx.spring_layout(G, seed=42)

    cores_finais = [node_colors.get(n, '#DDDDDD') for n in G.nodes()]

    nx.draw_networkx_nodes(G, pos, node_size=700, node_color=cores_finais, edgecolors='black')
    nx.draw_networkx_edges(G, pos, width=2)
    nx.draw_networkx_labels(G, pos, font_size=12, font_color='white', font_weight='bold')

    plt.title("Resultado do Algoritmo Welsh-Powell (Gerado via C)", fontsize=16, fontweight='bold')
    plt.axis('off')

    nome_saida = "grafo_colorido.png"
    plt.savefig(nome_saida, dpi=300)
    print(f"Sucesso! Imagem salva como '{nome_saida}'")

if __name__ == "__main__":
    desenhar_grafo()