<?php snippet('header') ?>

<div class="container">
  <h1><?= $page->title() ?></h1>
  <?php if ($page->tags()->isNotEmpty()) : ?>
    <?php foreach ($page->tags()->split() as $tag) : ?>
      <span class="badge badge-light"><?= $tag ?></span>
    <?php endforeach ?>
  <?php endif ?>

  <hr>

  <h4><?= $page->intro() ?></h4>

  <?= $page->text()->kt() ?>
</div>

<div class="container">
  <h4>À vous de jouer</h4>
  <p>(Peut-être mettre des filtres ici)</p>
  <ul>
    <?php foreach ($page->actions()->toStructure() as $action) : ?>
      <li>
        <?= $action->action() ?> avec <?= $action->org() ?> - 
        <a href="<?= $action->orglink() ?>" target="_blank">GO</a>
      </li>
    <?php endforeach ?>
  </ul>
</div>

<?php snippet('footer') ?>
